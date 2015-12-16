/*
   Copyright (c) 2013, The Linux Foundation. All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
   ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdlib.h>
#include <blkid/blkid.h>
#include <unistd.h>

#include "vendor_init.h"
#include "property_service.h"
#include "log.h"
#include "util.h"

/*
 *  The original partitions for system, data, and internal storage are
 *  by-name/system, by-name/userdata and by-name/fat, respectively.
 *  Some unofficial AOSP ROMs repurpose these three partitions to grant
 *  more room to the data partition. Detect whether these
 *  partitions have been swapped during init and if true:
 *  1) apply new device name 'evitaX'
 *  2) Change fstabs
 */

#define BLK_PART_ORIG_DATA "/dev/block/platform/msm_sdcc.1/by-name/userdata"
#define BLK_PART_ORIG_STOR "/dev/block/platform/msm_sdcc.1/by-name/fat"

void vendor_load_properties() {
    char platform[PROP_VALUE_MAX];
    char *detected_fs_type_data;
    char *detected_fs_type_stor;
    int rc;

    property_set("ro.product.device", "evitaX");
    property_set("ro.build.product", "evitaX");
    ERROR("Found swapped partition scheme; evitaX device name applied");

}
