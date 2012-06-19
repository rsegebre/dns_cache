//Lab01-Roberto Segebre. 
//(Worked with Avi and Oana)

#include <string>
#include <vector>
#include <netinet/in.h>
using namespace std;

class DnsCache {
public:
    DnsCache();
	~DnsCache();
    bool lookup(const string &s, struct in_addr &ip);
    void invalidate(const struct in_addr &ip);
    void invalidate(const string &s);
    double average_lookup();
    void dump_cache();

private:
    vector<struct domain_ip> cache;
	double average;
	int dns_cnt;
    };

struct domain_ip {
    string domain_name;
    struct in_addr ip_temp;
	};
