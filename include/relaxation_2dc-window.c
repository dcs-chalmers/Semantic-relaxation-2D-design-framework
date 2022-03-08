#include "relaxation_2dc-window.h"

descriptor_t put_window(DS_TYPE* set, uint8_t contention)
{	
	uint64_t hops, random;
	hops = random = 0;
	descriptor_t descriptor;
	
	if(contention == 1)
	{		
		thread_index == random_index(set);
		contention = 0;
	}
	
	if(thread_Window.version != global_Window.descriptor.version) 
	{
		thread_Window = global_Window.descriptor; 
	}
	
	while(1)
	{	
		/* shift window */
		if(hops == set->width) 
		{
			if(thread_Window.version == global_Window.descriptor.version)
			{
				new_window.max = thread_Window.max + set->shift;
				new_window.version = thread_Window.version + 1;
				if(CAE(&global_Window.descriptor,&thread_Window,&new_window))
				{
					my_slide_count+=1;
				}
				else
				{
					my_slide_fail_count+=1;
				}
			}
			thread_Window = global_Window.descriptor;
			hops = 0;
		}
		/* read descriptor */
		descriptor =  set->set_array[thread_index].descriptor;
		if(descriptor.count < global_Window.descriptor.max)
		{
			return descriptor;
		}
		/* hop */
		else if(thread_Window.version == global_Window.descriptor.version)
		{
			if(random < set->random_hops)
			{
				thread_index = random_index(set); 
				random += 1;
			}
			else
			{
				thread_index += 1;
				if(thread_index == set->width)
				{
					thread_index=0;
				}
			}
			hops += 1;
			my_hop_count+=1;
		}
		/* switch to current window */
		else
		{
			thread_Window = global_Window.descriptor; 
			hops = 0;
		}
	}
}
descriptor_t get_window(DS_TYPE* set, uint8_t contention)
{	
	uint64_t hops, random;
	hops = random = 0; 
	descriptor_t descriptor;
	uint8_t empty = 1;
	
	if(contention == 1)
	{		
		thread_index = random_index(set);
		contention = 0;
	}
	
	if(thread_Window.version != global_Window.descriptor.version)
	{
		thread_Window = global_Window.descriptor; 
	}
	
	while(1)
	{	
		/* shift window */
		if(hops == set->width)
		{
			/* emptiness check */
			if(empty == 1)
			{				
				return descriptor;
			}
			/*******************/
			/* thread_Window.max > set->depth, makes sure we are not in the last window before shifting down, this maintains (global_Window.descriptor.max >= set->depth) */
			if(thread_Window.version == global_Window.descriptor.version && thread_Window.max > set->depth)
			{
				new_window.max = thread_Window.max - set->shift;
				new_window.version = thread_Window.version + 1;
				if(CAE(&global_Window.descriptor,&thread_Window,&new_window))
				{
					my_slide_count+=1;
				}
				else
				{
					my_slide_fail_count+=1;
				}
			}
			thread_Window = global_Window.descriptor; 
			hops = 0; empty = 1;
		}
		/* read descriptor */
		descriptor =  set->set_array[thread_index].descriptor;
		/* empty sub-structures will be skipped at this point because (global_Window.descriptor.max - set->depth) cannot go bellow zero */
		if(descriptor.count > (global_Window.descriptor.max - set->depth))
		{
			return descriptor;
		}
		/* change index (hop) */
		else if(thread_Window.version == global_Window.descriptor.version)
		{
			/* emptiness check */
			if(descriptor.count > 0)
			{			
				empty = 0;
			}
			/*******************/
			if(random < set->random_hops)
			{
				thread_index = random_index(set); 
				random += 1;
			}
			else
			{					
				hops += 1;
				thread_index += 1;
				if(thread_index == set->width)
				{
					thread_index=0;
				}
			}
			my_hop_count+=1;
		}
		/* switch to current window */
		else
		{			
			thread_Window = global_Window.descriptor; 
			hops = 0; empty = 1; 
		}
	}
}
uint64_t random_index(DS_TYPE* set)
{
	return (my_random(&(seeds[0]), &(seeds[1]), &(seeds[2])) % (set->width));
}
