/*
 * Copyright(c) 2021 Intel Corporation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*!
 * @file port_mgr_dpdk_port.c
 *
 *
 * Definitions for DPDK Port Manager APIs.
 */

#include "port_mgr/port_mgr_port.h"
#include "lld/dpdk/lld_dpdk_port.h"
#include "port_mgr/port_mgr.h"

bf_status_t port_mgr_port_add(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
			      port_attributes_t *port_attrib)
{
	bf_status_t status = BF_SUCCESS;

	port_mgr_log_trace("Entering %s", __func__);

	/* Invoke LLD DPDK API to Add Port */
	status = lld_dpdk_port_add(dev_port, port_attrib);
	if (status != BF_SUCCESS)
		return status;

	/* Add the Port Info to Hash Map */
	status = port_mgr_set_port_info(dev_port, port_attrib);
	if (status != BF_SUCCESS)
		return status;

	port_mgr_log_trace("Exiting %s", __func__);

	return status;
}

bf_status_t port_mgr_port_all_stats_get(bf_dev_id_t dev_id,
					bf_dev_port_t dev_port,
					uint64_t *stats)
{
	port_info_t *port_info = NULL;
	bf_status_t status = BF_SUCCESS;

	port_mgr_log_trace("Entering %s", __func__);

	/* Get the Port Info from Hash Map */
	port_info = port_mgr_get_port_info(dev_port);
	if (!port_info) {
		port_mgr_log_error("%s: failed to get port info, dev port %d",
				   __func__, dev_port);
		status = BF_OBJECT_NOT_FOUND;
		return status;
	}

	/* Invoke LLD DPDK API to get Port Stats */
	status = lld_dpdk_port_stats_get(dev_port, &port_info->port_attrib,
					 stats);
	if (status != BF_SUCCESS)
		return status;

	port_mgr_log_trace("Exiting %s", __func__);

	return status;
}
