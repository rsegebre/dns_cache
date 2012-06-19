//Computer-Networks:
//Lab01-Roberto Segebre. 
//(Worked with Avi and Oana)
#include <vector>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <string.h>
#include "dnscache.hh"
using namespace std;

DnsCache::DnsCache(){
	//Initialize variables class variables.
	vector <struct domain_ip>  cache; 
	double average = 0;
	int dns_cnt =0;
	}

DnsCache::~DnsCache() {
	//no heap allocations that need to be deleted.
}

bool DnsCache::lookup(const string &s, struct in_addr &ip) {
	// Look into cache for a domain name
	// Start seeing how much time it takes to perform a look up in our cache. 
	struct timeval begin , end , diff;
	gettimeofday (&begin , 0);
	int cache_size = cache.size();
	for (int i = 0; i < cache_size; i++) {
		if (cache[i].domain_name.compare(s) == 0 )	{
			ip = cache[i].ip_temp;	
			gettimeofday (&end , 0);
			timersub (&end , &begin , &diff );
			dns_cnt++;
			double sec = diff.tv_sec + diff.tv_usec/1000000;
			average += sec;
			return true;
			}
		}	
	/*
	If domain_name not found in cache then perform DNS look-up:
	Joel's Code + some mods:
	DNS look it up.
	Note: the following code uses "hints" to getaddrinfo to specifically
	ask for just IPv4 addresses. It also only grabs the first IP address
	returned , and ignores any other additional addresses returned.
	*/
	string hostname = s;
	struct in_addr ipaddr;
	struct addrinfo *result = 0; // ptr to lookup results
	struct addrinfo hints; // tell getnameinfo a bit about what we 're looking for
	memset (&hints , 0, sizeof(hints)); // zero out the hints structure
	hints.ai_family = AF_INET; // we 're only interested in IPv4 addrs
	
	// DNS look-up time calculation
	int error = getaddrinfo (hostname.c_str (), 0, &hints , &result );
	gettimeofday (&end , 0);
	timersub (&end , &begin , &diff );
	dns_cnt++;
	double sec = diff.tv_sec + diff.tv_usec/1000000.0;
	average += sec;
	if (error == 0) {
		// use the first IP address obtained from the function call
		struct sockaddr_in *sin = (struct sockaddr_in *)( result->ai_addr );
		// put IP address in our ipaddr struct
		memcpy (& ipaddr , &sin ->sin_addr , sizeof(ipaddr ));
		// free up the memory consumed by DNS results
		freeaddrinfo (result );
		char ipstr [32];
		inet_ntop(AF_INET , &ipaddr , ipstr , 32);
		//Store struct in cache and make put ip address found in ip.
		struct domain_ip temp_addr;
		temp_addr.domain_name = s;
		temp_addr.ip_temp = ipaddr;
		cache.push_back(temp_addr);
		ip = ipaddr;	
		//Look-up suceeded!
		return true;
		}
	else {
		//Look-up failed
		return false;
		}
	}

void DnsCache::invalidate(const struct in_addr &ip) {
	//Iterate until ip match and erase
	vector<struct domain_ip>::iterator iter;
	for (iter = cache.begin(); iter != cache.end(); iter++) {
		if (iter->ip_temp.s_addr == ip.s_addr)	{
			cache.erase(iter);
			break;
			}
		}
	}

void DnsCache::invalidate(const string &s)  {
	//Iterate until domain name match and erase
	vector<struct domain_ip>::iterator iter;
	for (iter = cache.begin(); iter != cache.end(); iter++) {
		if (iter->domain_name.compare(s) == 0)	{
			cache.erase(iter);
			break;
			}
		}
	}

double DnsCache::average_lookup() {
	//If no lookups then we return 0.	
	if (dns_cnt == 0) return 0;	
	return double(average/dns_cnt);
	}

void DnsCache::dump_cache() {
	//Iterate + Fancy Formatting ;P
	vector<struct domain_ip>::iterator iter;
	cout << "------------------------------------------------------" << endl;
	for (iter = cache.begin(); iter != cache.end(); iter++) {
		char buffer[32];
	 	inet_ntop(AF_INET, &iter->ip_temp.s_addr, buffer, 32);
		cout << "Domain Name--->" << iter->domain_name << "	IP Addr--->" << buffer << endl;
		}
	cout << "------------------------------------------------------" << endl;
	}
