/*-
 * Copyright 2018 Cryply team
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file was originally written by Cryply team as part of the Cryply
 * coin.
 */
#include "yespower.h"

#include "algo-gate-api.h"

int verstring;

void yespower_hash( const char *input, char *output, uint32_t len )
{
   if (verstring==1)
   { 
		static const yespower_params_t v1 = {YESPOWER_0_9, 2048, 32, NULL, 0};
		yespower_tls( (yespower_binary_t*)input, len, &v1, (yespower_binary_t*)output ); 
	}
   if (verstring==2)
   {
				static const yespower_params_t v2 = {YESPOWER_0_9, 4096, 16, NULL, 0};
				yespower_tls( (yespower_binary_t*)input, len, &v2, (yespower_binary_t*)output ); 
	}
   if (verstring==3)
   {
				static const yespower_params_t v3 = {YESPOWER_0_9, 2048, 32, "CPUpower: The number of CPU working or available for proof-of-work mining", 73};
				yespower_tls( (yespower_binary_t*)input, len, &v3, (yespower_binary_t*)output ); 
	}
   if (verstring==4)
   {
				static const yespower_params_t v4 = {YESPOWER_0_9, 2048, 32, (const uint8_t *)"UraniumX", 8};
				yespower_tls( (yespower_binary_t*)input, len, &v4, (yespower_binary_t*)output ); 
	}
   if (verstring==5)
   {
				static const yespower_params_t v5 = {YESPOWER_0_9, 2048, 32, "LITBpower: The number of LITB working or available for proof-of-work mining", 73};
				yespower_tls( (yespower_binary_t*)input, len, &v5, (yespower_binary_t*)output ); 
	}	
   if (verstring==6)
   {
				static const yespower_params_t v6 = {YESPOWER_0_9, 2048, 32, "InterITC", 8};
				yespower_tls( (yespower_binary_t*)input, len, &v6, (yespower_binary_t*)output ); 
	}	
   if (verstring==7)
   {
				static const yespower_params_t v7 = {YESPOWER_0_9, 2048, 32, "Satoshi Nakamoto 31/Oct/2008 Proof-of-work is essentially one-CPU-one-vote", 74};
				yespower_tls( (yespower_binary_t*)input, len, &v7, (yespower_binary_t*)output ); 
	}		
}

int scanhash_yespower( int thr_id, struct work *work, uint32_t max_nonce,
                       uint64_t *hashes_done )
{
        uint32_t _ALIGN(64) vhash[8];
        uint32_t _ALIGN(64) endiandata[20];
        uint32_t *pdata = work->data;
        uint32_t *ptarget = work->target;

        const uint32_t Htarg = ptarget[7];
        const uint32_t first_nonce = pdata[19];
        uint32_t n = first_nonce;

        for (int k = 0; k < 19; k++)
                be32enc(&endiandata[k], pdata[k]);

        do {
                be32enc(&endiandata[19], n);
                yespower_hash((char*) endiandata, (char*) vhash, 80);
                if (vhash[7] < Htarg && fulltest(vhash, ptarget)) {
                        work_set_target_ratio( work, vhash );
                        *hashes_done = n - first_nonce + 1;
                        pdata[19] = n;
                        return true;
                }
                n++;
        } while (n < max_nonce && !work_restart[thr_id].restart);

        *hashes_done = n - first_nonce + 1;
        pdata[19] = n;

        return 0;
}

int64_t yespower_get_max64()
{
  return 0xfffLL;
}

bool register_yespower_algo( algo_gate_t* gate )
{
  gate->optimizations = SSE2_OPT | SHA_OPT;
  gate->get_max64     = (void*)&yespower_get_max64;
  gate->scanhash      = (void*)&scanhash_yespower;
  gate->hash          = (void*)&yespower_hash;
  gate->set_target    = (void*)&scrypt_set_target;
  verstring=1;
  return true;
};

bool register_yespowerr16_algo( algo_gate_t* gate )
{
  gate->optimizations = SSE2_OPT | SHA_OPT;
  gate->get_max64     = (void*)&yespower_get_max64;
  gate->scanhash      = (void*)&scanhash_yespower;
  gate->hash          = (void*)&yespower_hash;
  gate->set_target    = (void*)&scrypt_set_target;
  verstring=2;
  return true;
};

bool register_cpupower_algo( algo_gate_t* gate )
{
  gate->optimizations = SSE2_OPT | SHA_OPT;
  gate->get_max64     = (void*)&yespower_get_max64;
  gate->scanhash      = (void*)&scanhash_yespower;
  gate->hash          = (void*)&yespower_hash;
  gate->set_target    = (void*)&scrypt_set_target;
  verstring=3;
  return true;
};

bool register_yespowerurx_algo( algo_gate_t* gate )
{
  gate->optimizations = SSE2_OPT | SHA_OPT;
  gate->get_max64     = (void*)&yespower_get_max64;
  gate->scanhash      = (void*)&scanhash_yespower;
  gate->hash          = (void*)&yespower_hash;
  gate->set_target    = (void*)&scrypt_set_target;
  verstring=4;
  return true;
};

bool register_yespowerlitb_algo( algo_gate_t* gate )
{
  gate->optimizations = SSE2_OPT | SHA_OPT;
  gate->get_max64     = (void*)&yespower_get_max64;
  gate->scanhash      = (void*)&scanhash_yespower;
  gate->hash          = (void*)&yespower_hash;
  gate->set_target    = (void*)&scrypt_set_target;
  verstring=5;
  return true;
};

bool register_yespowerinter_algo( algo_gate_t* gate )
{
  gate->optimizations = SSE2_OPT | SHA_OPT;
  gate->get_max64     = (void*)&yespower_get_max64;
  gate->scanhash      = (void*)&scanhash_yespower;
  gate->hash          = (void*)&yespower_hash;
  gate->set_target    = (void*)&scrypt_set_target;
  verstring=6;
  return true;
};

bool register_yespowersugar_algo( algo_gate_t* gate )
{
  gate->optimizations = SSE2_OPT | SHA_OPT;
  gate->get_max64     = (void*)&yespower_get_max64;
  gate->scanhash      = (void*)&scanhash_yespower;
  gate->hash          = (void*)&yespower_hash;
  gate->set_target    = (void*)&scrypt_set_target;
  verstring=7;
  return true;
};

bool register_yespoweradvc_algo( algo_gate_t* gate )
{
  gate->optimizations = SSE2_OPT | SHA_OPT;
  gate->get_max64     = (void*)&yespower_get_max64;
  gate->scanhash      = (void*)&scanhash_yespower;
  gate->hash          = (void*)&yespower_hash;
  gate->set_target    = (void*)&scrypt_set_target;
  verstring=7;
  return true;
};

bool register_yespowereqpay_algo( algo_gate_t* gate )
{
  gate->optimizations = SSE2_OPT | SHA_OPT;
  gate->get_max64     = (void*)&yespower_get_max64;
  gate->scanhash      = (void*)&scanhash_yespower;
  gate->hash          = (void*)&yespower_hash;
  gate->set_target    = (void*)&scrypt_set_target;
  verstring=7;
  return true;
};

bool register_yespowerltncg_algo( algo_gate_t* gate )
{
  gate->optimizations = SSE2_OPT | SHA_OPT;
  gate->get_max64     = (void*)&yespower_get_max64;
  gate->scanhash      = (void*)&scanhash_yespower;
  gate->hash          = (void*)&yespower_hash;
  gate->set_target    = (void*)&scrypt_set_target;
  verstring=7;
  return true;
};

bool register_yespowermgpc_algo( algo_gate_t* gate )
{
  gate->optimizations = SSE2_OPT | SHA_OPT;
  gate->get_max64     = (void*)&yespower_get_max64;
  gate->scanhash      = (void*)&scanhash_yespower;
  gate->hash          = (void*)&yespower_hash;
  gate->set_target    = (void*)&scrypt_set_target;
  verstring=7;
  return true;
};

bool register_yespowertide_algo( algo_gate_t* gate )
{
  gate->optimizations = SSE2_OPT | SHA_OPT;
  gate->get_max64     = (void*)&yespower_get_max64;
  gate->scanhash      = (void*)&scanhash_yespower;
  gate->hash          = (void*)&yespower_hash;
  gate->set_target    = (void*)&scrypt_set_target;
  verstring=7;
  return true;
};
