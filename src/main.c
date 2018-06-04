/*
 * The GPL License (GPLv3)
 *
 * Copyright © 2016 Thomas "Ventto" Venriès  <thomas.venries@gmail.com>
 *
 * Pearlfan is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Pearlfan is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Pearlfan.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <libusb-1.0/libusb.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "config.h"
#include "draw.h"
#include "defutils.h"
#include "optutils.h"
#include "raster.h"
#include "usb.h"

int main(int argc, char **argv)
{
    pfan_opts opts;
    int ret;

    if ((ret = pfan_getopt(argc, argv, &opts)) != PFAN_VALID_OPT) {
        return ret;
    }

    char    img_paths[PFAN_MAX_DISPLAY][MAX_PATH];
    int     img_nbr;
    uint8_t effects[PFAN_MAX_DISPLAY][3];

    memset(effects, 0x00, sizeof(effects));

    if (opts.cflag &&
        (img_nbr = pfan_read_cfg(opts.carg, img_paths, effects)) < 0) {
        fprintf(stderr, "Invalid config file.\n\n");
        return 1;
    }

    if (opts.dflag &&
        (img_nbr = pfan_read_dir(opts.darg, img_paths, effects)) < 0) {
        fprintf(stderr, "Can not open '%s' directory.\n\n", opts.darg);
        return 1;
    }

    uint8_t **rasters = NULL;
    uint16_t displays[PFAN_MAX_DISPLAY][PFAN_MAX_W];

    memset(displays, 0xFF, sizeof(displays));

    if (!opts.tflag) {
        if (!img_nbr) {
            fprintf(stdout, "No .PBM image found.\n\n");
            return 0;
        }
        rasters = pfan_create_img_rasters(img_paths, img_nbr);
        if (!rasters) {
            return 1;
        }

        for (int i = 0; i < img_nbr ; i++) {
            pfan_draw_image(rasters[i], displays[i]);
        }
    }

    if (opts.tflag) {
        img_nbr = pfan_draw_paragraph(opts.targ, displays);
        /* Not enough space to display text */
        if (!img_nbr) {
            return 1;
        }

        for(int i = 0; i < img_nbr; ++i) {
            effects[i][2] = 0x02;
        }
    }

    if (opts.fflag) {
        memset(effects, 0x06, sizeof(effects));
    }

    fprintf(stdout, "\n");

    libusb_device_handle *usb_handle = NULL;

    if (libusb_init(NULL) < 0) {
        if (rasters) {
            pfan_free_rasters(rasters, img_nbr);
        }

        fprintf(stderr, "Libusb initialization failed.\n\n");
        return 1;
    }

    usb_handle = pfan_open(PFAN_VID, PFAN_PID);
    if (!usb_handle) {
        libusb_exit(NULL);

        if (rasters) {
            pfan_free_rasters(rasters, img_nbr);
        }

        return 1;
    }

    fprintf(stdout, "Device found.\n");
    fprintf(stdout, "Transfer started.\n");

    int expected_transfer = 320 * img_nbr;
    int bytes = pfan_send(usb_handle, img_nbr, effects, displays);

    ret = 0;

    if (bytes < expected_transfer) {
        fprintf(stderr, "Transfer aborted.\n");

        if (bytes < 0) {
            fprintf(stderr, "Transfer error (libusberr = %d)\n\n", bytes);
        }

        fprintf(stderr, "( %d / %d bytes)\n\n", bytes, expected_transfer);
        ret = 1;
    }

    fprintf(stdout, "Transfer is complete.\n\n");

    pfan_close(usb_handle);

    if (rasters) {
        pfan_free_rasters(rasters, img_nbr);
    }

    return ret;
}
