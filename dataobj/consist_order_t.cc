/* This file is part of the Simutrans project under the artistic licence.
* (see licence.txt)
*
* @author: jamespetts, February 2018
*/

#include "consist_order_t.h"

#include "loadsave.h"
#include "translator.h"

#include "../descriptor/vehicle_desc.h"
#include "../bauer/vehikelbauer.h"

void consist_order_t::rdwr(loadsave_t* file)
{
	file->rdwr_short(tags_to_clear);

	uint32 orders_count = orders.get_count();
	file->rdwr_long(orders_count);

	if(file->is_saving())
	{
		for(auto order : orders)
		{
			file->rdwr_byte(order.catg_index);
			file->rdwr_bool(order.clear_all_tags);
			file->rdwr_short(order.tags_required);
			file->rdwr_short(order.tags_to_set);

			uint32 vehicle_description_count = order.vehicle_description.get_count();
			file->rdwr_long(vehicle_description_count); 

			for(auto vehicle_description : order.vehicle_description)
			{
				// Save each element of the vehicle descriptors
				const char *s = vehicle_description.specific_vehicle ? vehicle_description.specific_vehicle->get_name() : "~BLANK~"; 
				file->rdwr_str(s);

				file->rdwr_bool(vehicle_description.empty);
				file->rdwr_byte(vehicle_description.engine_type);
				file->rdwr_byte(vehicle_description.min_catering);
				file->rdwr_byte(vehicle_description.max_catering);
				file->rdwr_byte(vehicle_description.must_carry_class);
				file->rdwr_long(vehicle_description.min_range); 
				file->rdwr_long(vehicle_description.max_range);
				file->rdwr_short(vehicle_description.min_brake_force);
				file->rdwr_short(vehicle_description.max_brake_force);
				file->rdwr_long(vehicle_description.min_power);
				file->rdwr_long(vehicle_description.max_power);
				file->rdwr_long(vehicle_description.min_tractive_effort);
				file->rdwr_long(vehicle_description.max_tractive_effort);
				file->rdwr_long(vehicle_description.max_topspeed);
				file->rdwr_long(vehicle_description.min_topspeed);
				file->rdwr_long(vehicle_description.max_weight);
				file->rdwr_long(vehicle_description.min_weight);
				file->rdwr_long(vehicle_description.max_axle_load);
				file->rdwr_long(vehicle_description.min_axle_load);
				file->rdwr_short(vehicle_description.min_capacity);
				file->rdwr_short(vehicle_description.max_capacity);
				file->rdwr_long(vehicle_description.max_running_cost);
				file->rdwr_long(vehicle_description.min_running_cost); 
				file->rdwr_long(vehicle_description.max_fixed_cost);
				file->rdwr_long(vehicle_description.min_fixed_cost);

				for(uint32 i = 0; i < vehicle_description_element::max_rule_flags; i ++)
				{
					file->rdwr_short(vehicle_description.rule_flags[i]); 
				}
			}
		}
	}
	
	if (file->is_loading())
	{
		for (uint32 i = 0; i < orders_count; i++)
		{
			// Load each order
			consist_order_element_t order;

			file->rdwr_byte(order.catg_index);
			file->rdwr_bool(order.clear_all_tags);
			file->rdwr_short(order.tags_required);
			file->rdwr_short(order.tags_to_set);

			uint32 vehicle_description_count = 0;
			file->rdwr_long(vehicle_description_count); 

			for(uint32 j = 0; j < vehicle_description_count; j ++)
			{
				vehicle_description_element vehicle_description;

				char vehicle_name[512];
				file->rdwr_str(vehicle_name, 512);
				const vehicle_desc_t* desc = vehicle_builder_t::get_info(vehicle_name);
				if(desc == nullptr) 
				{
					desc = vehicle_builder_t::get_info(translator::compatibility_name(vehicle_name));
				}
				vehicle_description.specific_vehicle = desc;

				file->rdwr_bool(vehicle_description.empty);
				file->rdwr_byte(vehicle_description.engine_type);
				file->rdwr_byte(vehicle_description.min_catering);
				file->rdwr_byte(vehicle_description.max_catering);
				file->rdwr_byte(vehicle_description.must_carry_class);
				file->rdwr_long(vehicle_description.min_range); 
				file->rdwr_long(vehicle_description.max_range);
				file->rdwr_short(vehicle_description.min_brake_force);
				file->rdwr_short(vehicle_description.max_brake_force);
				file->rdwr_long(vehicle_description.min_power);
				file->rdwr_long(vehicle_description.max_power);
				file->rdwr_long(vehicle_description.min_tractive_effort);
				file->rdwr_long(vehicle_description.max_tractive_effort);
				file->rdwr_long(vehicle_description.max_topspeed);
				file->rdwr_long(vehicle_description.min_topspeed);
				file->rdwr_long(vehicle_description.max_weight);
				file->rdwr_long(vehicle_description.min_weight);
				file->rdwr_long(vehicle_description.max_axle_load);
				file->rdwr_long(vehicle_description.min_axle_load);
				file->rdwr_short(vehicle_description.min_capacity);
				file->rdwr_short(vehicle_description.max_capacity);
				file->rdwr_long(vehicle_description.max_running_cost);
				file->rdwr_long(vehicle_description.min_running_cost); 
				file->rdwr_long(vehicle_description.max_fixed_cost);
				file->rdwr_long(vehicle_description.min_fixed_cost);

				for(uint32 i = 0; i < vehicle_description_element::max_rule_flags; i ++)
				{
					file->rdwr_short(vehicle_description.rule_flags[i]); 
				}

				order.vehicle_description.append(vehicle_description);
			}

			orders.append(order);
		}
	}
}