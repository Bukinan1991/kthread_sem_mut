#include <linux/slab.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

struct test
{
    struct list_head st;
};

void test_lists(void)
{
    