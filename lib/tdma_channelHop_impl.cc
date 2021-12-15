/* -*- c++ -*- */
/* 
 * Copyright 2021 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "tdma_channelHop_impl.h"
#include <cstdio>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdexcept>
#include <string.h>
#include <iostream>
#include <vector>

//libraries for shared memory
#include <sys/ipc.h> 
#include <sys/shm.h> 
uint64_t *ASN;
key_t keyasn;
int shmidasn;
uint8_t d_freq_seq[16];

namespace gr {
  namespace Tsch_4e {

    tdma_channelHop::sptr
    tdma_channelHop::make(pmt::pmt_t msg, float period_ms, int slot_length, std::vector<uint8_t> freq_seq)
    {
      return gnuradio::get_initial_sptr
        (new tdma_channelHop_impl(msg, period_ms, slot_length, freq_seq));
    }

    /*
     * The private constructor
     */
    tdma_channelHop_impl::tdma_channelHop_impl(pmt::pmt_t msg, float period_ms, int slot_length, std::vector<uint8_t> freq_seq)
      : gr::block("tdma_channelHop",
             gr::io_signature::make(0,0,0),//gr::io_signature::make(1, 1, sizeof(float))
              gr::io_signature::make(0, 0, 0)),
	      d_finished(false),
              d_period_ms(period_ms),
              d_msg(msg),
              d_slot_length(slot_length)

    {   
        int i=0;
        for (auto it = freq_seq.begin(); it != freq_seq.end(); it++) {
                 d_freq_seq[i]=*it;
                  i+=1;
        }     

        /*if(freq_seq.size() >16)
			throw std::invalid_argument("Sequency should be less than or eaqual to 16");
        else
             {
		for(int i=0;i<=freq_seq.size();i++)
		          d_freq_seq[i] = freq_seq[i];
             }*/
        message_port_register_out(pmt::mp("strobe"));
	message_port_register_out(pmt::mp("strobe_data"));
	message_port_register_in(pmt::mp("set_msg"));
        set_msg_handler(pmt::mp("set_msg"),
        boost::bind(&tdma_channelHop_impl::set_msg, this, _1));
	//ASN implementation
	//adding shared memory formation
	// ftok to generate unique key
	keyasn = ftok("shmfile",65);
	// shmget returns an identifier in shmid 
	shmidasn = shmget(keyasn,sizeof(uint64_t),0666|IPC_CREAT);
	// shmat to attach to shared memory
	ASN=(uint64_t*) shmat(shmidasn,(void*)0,0);
	//detach from shared memory 
	shmdt(ASN);
    }

    /*
     * Our virtual destructor.
     */
    tdma_channelHop_impl::~tdma_channelHop_impl()
    {
    }

    bool
    tdma_channelHop_impl::start()
    {
      // NOTE: d_finished should be something explicitely thread safe. But since
      // nothing breaks on concurrent access, I'll just leave it as bool.
      d_finished = false;
      d_thread = boost::shared_ptr<gr::thread::thread>
        (new gr::thread::thread(boost::bind(&tdma_channelHop_impl::run, this)));

      return block::start();
    }

	bool
    tdma_channelHop_impl::stop()
    {
      // Shut down the thread
      d_finished = true;
      d_thread->interrupt();
      d_thread->join();

      return block::stop();
    }

	void tdma_channelHop_impl::run()
    {
      while(!d_finished) {
	//std::cout<<"Giving a delay"<<d_period_ms<<"\n";
	//set_period(2000);//To set the time period according to the extracted frame
        boost::this_thread::sleep(boost::posix_time::milliseconds(d_period_ms));
        if(d_finished) {
          return;
        }
       //try adding ASN here and the frequency division also and d_msg=pmt.to_pmt({'ASN': asn value,'freq': freq_msg, 'lo_offset': lo_msg}); address of the receiver can be changed according to the asn make asn as a shared memory variable no need to send it as a msg through the ports 
        //*ASN+=1 
	//ASN_lsb=(uint8_t*) shmat(shmidlsb,(void*)0,0);
	//ASN_msb=(uint32_t*) shmat(shmidmsb,(void*)0,0);
	//adding SM implementation
        // shmat to attach to shared memory
        ASN=(uint64_t*) shmat(shmidasn,(void*)0,0);
	if(*ASN !=0x0000010000000000)
 		*ASN+=1;
	else
	*ASN=0x0000000000000000;
	int channel[]={11,15,19,23,27,22,16,14,13,26,12,26,17,25,18,20};
	uint8_t choffset=0x01;//(rand() % 16);
	double i=0;
	i=(*ASN+choffset)%0x04;
	/*if(*ASN_lsb!=0xFF)//checking wether lsb has reached it's max value
	{   std::cout<<"IN ASN LSB"<<std::endl;
	    std::cout<<"ASN LSB before"<<std::hex<<(int)*ASN_lsb<<std::endl;
           *ASN_lsb=*ASN_lsb+1;//incrementing the lsb
	   *ASN=0x0000000000000000 or *ASN_lsb;//actual asn should be 5 bytes here we are appending the lsb to asn
	std::cout<<"ASN LSB"<<std::hex<<(int)*ASN_lsb<<std::endl;
           i=(*ASN+choffset)%0x04;
	}message_port_pub
	else{
	*ASN=0x0000000000000000; 
	*ASN_msb+=0x01;//incrementing the msb i.e 4 bytes from the msb
	*ASN=*ASN_msb;
        *ASN=*ASN<<8 or 0xFF; //Appending the lsb of asn(1 byte)
        i=(*ASN+choffset)%0x04;	
	}*/
	

	//frequency hopping
	 double freq=1000000L * (2405 + 5 * (channel[(int)i] - 11)); //uncomment for freq hopping
	//double freq = 2.400*1000000000L; //for static channel no hopping  
       //std::cout << "ASN as diplayed in the Message Strobe = " << (int)*ASN << std::endl;//uncomment after video is done
	std::cout<<"\n";//comment after video is done
	std::cout<<"*********************************************************************************"<<"\n";//comment after video is done
	std::cout<<"OUT OF TREE MODULE Initializing"<<"\n";//comment after video is done
        std::cout << "Slot Length = " <<d_slot_length<< std::endl;
	std::cout<<"Setting frequency"<<d_freq_seq[1]<<"\n";//comment aftre video is done
        std::cout << "Current Frequency value = " <<freq<< std::endl;
       //detach from shared memory 
	shmdt(ASN); 
       /*shmdt(ASN_lsb);
       shmdt(ASN_msb);
       shmdt(ASN);*/
       message_port_pub(pmt::mp("strobe_data"), d_msg); 
       //std::cout<<"Setting frequency"<<"\n";//uncomment after video is done
	std::cout<<"OUT OF TREE MODULE : Ready To Send/Receive Packets"<<"\n";//comment after video is done
       //std::cout<<"MESSAGE STROBE : Ready To Send/Receive Packets"<<"\n";//uncomment after video is done
       pmt::pmt_t d_msg_freq=pmt::cons(pmt::string_to_symbol("freq"),pmt::mp(freq));
       message_port_pub(pmt::mp("strobe"), d_msg_freq);
      }
    }

  } /* namespace Tsch_4e */
} /* namespace gr */

