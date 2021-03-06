/*
 * Copyright (C) 2007 Red Hat, Inc. All rights reserved.
 *
 * This file is part of LVM2.
 *
 * This copyrighted material is made available to anyone wishing to use,
 * modify, copy, or redistribute it subject to the terms and conditions
 * of the GNU Lesser General Public License v.2.1.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/*
 * Return the address of the last file name component of NAME.
 * If NAME ends in a slash, return the empty string.
 */

#include <string.h>

static inline const char *last_path_component(char const *name)
{
	char const *slash = strrchr(name, '/');

	return (slash) ? slash + 1 : name;
}
