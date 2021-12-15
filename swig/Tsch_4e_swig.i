/* -*- c++ -*- */

#define TSCH_4E_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "Tsch_4e_swig_doc.i"

%{
#include "Tsch_4e/tdma_channelHop.h"
%}


%include "Tsch_4e/tdma_channelHop.h"
GR_SWIG_BLOCK_MAGIC2(Tsch_4e, tdma_channelHop);
