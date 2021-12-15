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


#ifndef INCLUDED_TSCH_4E_TDMA_CHANNELHOP_H
#define INCLUDED_TSCH_4E_TDMA_CHANNELHOP_H

#include <Tsch_4e/api.h>
#include <gnuradio/block.h>

#include <vector>

namespace gr {
  namespace Tsch_4e {

    /*!
     * \brief <+description of block+>
     * \ingroup Tsch_4e
     *
     */
    class TSCH_4E_API tdma_channelHop : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<tdma_channelHop> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of Tsch_4e::tdma_channelHop.
       *
       * To avoid accidental use of raw pointers, Tsch_4e::tdma_channelHop's
       * constructor is in a private implementation
       * class. Tsch_4e::tdma_channelHop::make is the public interface for
       * creating new instances.
       */
      static sptr make(pmt::pmt_t msg, float period_ms, int slot_length, std::vector<uint8_t> freq_seq);

      virtual void set_msg(pmt::pmt_t msg) = 0;
      /*!
       * Get the value of the message being sent.
       */
      virtual pmt::pmt_t msg() const = 0;

      /*!
       * Reset the sending interval.
       * \param period_ms the time period in milliseconds.
       */
      virtual void set_period(float period_ms) = 0;

      /*!
       * Get the time interval of the strobe.
       */
      virtual float period() const = 0;

      virtual void set_slot_length(int slot_length) = 0;
     
      virtual int slot_length_value() const =0;

    };

  } // namespace Tsch_4e
} // namespace gr

#endif /* INCLUDED_TSCH_4E_TDMA_CHANNELHOP_H */

