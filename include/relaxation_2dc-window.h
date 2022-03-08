typedef ALIGNED(CACHE_LINE_SIZE) struct window_descriptor
{
	uint64_t max;
	uint64_t version; 
}windowd_t;

typedef ALIGNED(CACHE_LINE_SIZE) struct window_struct
{
	windowd_t descriptor;
	uint8_t padding[CACHE_LINE_SIZE - sizeof(windowd_t)]; 
}window_t;

/*window variables*/
window_t global_Window;

__thread windowd_t thread_Window;
__thread windowd_t new_window;
__thread uint64_t thread_index;

/*functions, descriptor_t defined within the data structure header file*/
descriptor_t put_window(DS_TYPE* set, uint8_t contention);
descriptor_t get_window(DS_TYPE* set, uint8_t contention);
uint64_t random_index(DS_TYPE* set);