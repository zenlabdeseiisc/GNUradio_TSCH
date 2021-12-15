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

#ifndef INCLUDED_TSCH_4E_TDMA_CHANNELHOP_IMPL_H
#define INCLUDED_TSCH_4E_TDMA_CHANNELHOP_IMPL_H

#include <Tsch_4e/tdma_channelHop.h>

namespace gr {
  namespace Tsch_4e {

    class tdma_channelHop_impl : public tdma_channelHop
    {
     private:
      boost::shared_ptr<gr::thread::thread> d_thread;
      bool d_finished;
      float d_period_ms;
      pmt::pmt_t d_msg;
      int d_slot_length;
      void run();

     public:
      tdma_channelHop_impl(pmt::pmt_t msg, float period_ms, int slot_length, std::vector<uint8_t> freq_seq);
      ~tdma_channelHop_impl();

      // Where all the action really happens
      void set_msg(pmt::pmt_t msg) { d_msg = msg; }
      pmt::pmt_t msg() const { return d_msg; }
      void set_period(float period_ms) { d_period_ms = period_ms; }
      float period() const { return d_period_ms; }
      void set_slot_length(int slot_length) { d_slot_length = slot_length; }
      int slot_length_value() const { return d_slot_length; }
      
      // Overloading these to start and stop the internal thread that
      // periodically produces the message.
      bool start();
      bool stop();
    };

  } // namespace Tsch_4e
} // namespace gr

#endif /* INCLUDED_TSCH_4E_TDMA_CHANNELHOP_IMPL_H */

