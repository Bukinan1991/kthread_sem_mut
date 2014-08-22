#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/netfilter.h>
#include <linux/skbuff.h>
#include <linux/tcp.h>
#include <linux/ip.h>
#include <linux/netfilter_ipv4.h>
#include <net/tcp.h>
#include <linux/udp.h>
struct nf_hook_ops bundele;

unsigned int hook_func(unsigned int hooknum, struct sk_buff *skb, const struct net_device *in, const struct net_device *out, int (*okfn)(struct sk_buff *))
{
    struct iphdr *ip;
    struct tcphdr *tcp;
    //struct udphdr *udp;
    if(skb->protocol == htons(ETH_P_IP))
    {
	ip = (struct iphdr *)skb_network_header(skb);
	if(ip->version == 4 && ip->protocol == IPPROTO_TCP)
	//if(ip->version == 4 && ip->protocol == IPPROTO_UDP)
	{
	    tcp = (struct tcphdr *)skb_transport_header(skb);
	    //udp = (struct udphdr *)skb_transport_header(skb);
	    if(tcp->dest == htons(9999))
	    //if(udp->dest == htons(9999))
	    {
		//printk(KERN_INFO "%i\n", ntohs(udp->source));
		tcp->dest = htons(9998);
	//	udp->dest = htons(9998);
		//skb_checksum_start_offset(skb);
		skb_checksum_complete(skb);
	    }
	}
	printk(KERN_INFO "exit\n");
    }
    return NF_ACCEPT;
}

int __init net_init(void)
{
    bundele.hook = hook_func;
    bundele.owner = THIS_MODULE;
    bundele.pf = PF_INET;
    bundele.hooknum = NF_INET_PRE_ROUTING;
    bundele.priority = NF_IP_PRI_FIRST;
    nf_register_hook(&bundele);
    return 0;
}

void __exit net_exit(void)
{
    nf_unregister_hook(&bundele);
}

module_init(net_init);
module_exit(net_exit);

MODULE_AUTHOR("Author");
MODULE_LICENSE("GPL");
