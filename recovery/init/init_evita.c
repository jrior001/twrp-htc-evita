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
 *  more room to the data partition. We are choosing to detect a change
 *  to the stock system partition. If swapped to vfat,
 *  1) apply new device name 'evitaX'
 *  2) Change fstabs
 *  otherwise continue loading standard device name/fstab
 */

#define BLK_PART_ORIG_SYS "/dev/block/platform/msm_sdcc.1/by-name/system"

void vendor_load_properties() {
    char platform[PROP_VALUE_MAX];
    char *detected_fs_type_sys;
    int rc;

    rc = property_get("ro.board.platform", platform);
    if (!rc || strncmp(platform, ANDROID_TARGET, PROP_VALUE_MAX)) {
    property_set("ro.product.device", "evita");
    property_set("ro.build.product", "evita");
        return;
    }

    detected_fs_type_sys = blkid_get_tag_value(NULL, "TYPE", BLK_PART_ORIG_SYS);
    if (detected_fs_type_sys == NULL) {
        /*
         * Under any fault case apply default the
         * default device name.
         */
        property_set("ro.product.device", "evita");
        property_set("ro.build.product", "evita");
        return;
    }

    /* detected_fs_type_sys = blkid_get_tag_value(NULL, "TYPE", BLK_PART_ORIG_SYS); */
    /*
     * FIXME: Until a more reliable check can be performed, we rely
     * on the fstype of by-name/sys to tell us if the swapped
     * partition scheme is active. Users will be instructed to
     * format /system accordingly to switch partition schemes.
     * This avoids issues with detecting fs type on encrypted fs.
     */

    if (strcmp(detected_fs_type_sys, "vfat") == 0 ) {
        property_set("ro.product.device", "evitaX");
        property_set("ro.build.product", "evitaX");
        symlink((const char *)("/etc/recovery.fstab.evitaX"),
                (const char *)("/etc/twrp.fstab"));
        ERROR("Found swapped partition scheme; evitaX device name applied");
    } else {
        property_set("ro.product.device", "evita");
        property_set("ro.build.product", "evita");
        ERROR("Found standard partition scheme; evita device name applied");
    }
}

