/*
 * Copyright (C) 2001  Sistina Software
 *
 * LVM is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * LVM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LVM; see the file COPYING.  If not, write to
 * the Free Software Foundation, 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 */

#include "tools.h"

static int vgexport_single(const char *vg_name);

int vgexport(int argc, char **argv)
{
	if (!argc && !arg_count(all_ARG)) {
		log_error("Please supply volume groups or use -a for all.");
		return ECMD_FAILED;
	}

	if (argc && arg_count(all_ARG)) {
		log_error("No arguments permitted when using -a for all.");
		return ECMD_FAILED;
	}

	return process_each_vg(argc, argv, LCK_READ, &vgexport_single);
}

static int vgexport_single(const char *vg_name)
{
	struct volume_group *vg;

	if (!(vg = fid->ops->vg_read(fid, vg_name))) {
		log_error("Unable to find volume group \"%s\"", vg_name);
		goto error;
	}

	if (vg->status & EXPORTED_VG) {
		log_error("Volume group \"%s\" is already exported", vg_name);
		goto error;
	}

	if (!(vg->status & LVM_WRITE)) {
		log_error("Volume group \"%s\" is read-only", vg_name);
		goto error;
	}

	if (lvs_in_vg_activated(vg)) {
		log_error("Volume group \"%s\" has active logical volumes", 
			  vg_name);
		goto error;
	}

	if (!archive(vg))
		goto error;

	vg->status |= EXPORTED_VG;

	if (!fid->ops->vg_write(fid,vg))
		goto error;

	backup(vg);

	log_print("Volume group \"%s\" successfully exported", vg->name);
	
	return 0;

     error:
	return ECMD_FAILED;
}
