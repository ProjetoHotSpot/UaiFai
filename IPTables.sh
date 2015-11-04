# Allow DHCP and DNS requests from any user:
iptables -t nat -A prerouting_rule -m state --state NEW,ESTABLISHED,RELATED,INVALID -p udp --dport 67 -j ACCEPT
iptables -t nat -A prerouting_rule -m state --state NEW,ESTABLISHED,RELATED,INVALID -p tcp --dport 67 -j ACCEPT
iptables -t nat -A prerouting_rule -m state --state NEW,ESTABLISHED,RELATED,INVALID -p udp --dport 53 -j ACCEPT
iptables -t nat -A prerouting_rule -m state --state NEW,ESTABLISHED,RELATED,INVALID -p tcp --dport 53 -j ACCEPT

# All other traffic goes to the portal page: 
iptables -t nat -A prerouting_rule -p tcp -j DNAT --to 10.0.0.1:80
iptables -t nat -A prerouting_rule -p udp -j DNAT --to 10.0.0.1:80

# Create a captive NET where approved users will go and we have full control of how they behave:
iptables -t nat -N NET
iptables -t nat -A PREROUTING -j NET
iptables -t nat -A NET -j ACCEPT

# Add a user with MAC address of 00:90:4B:B1:5A:75.
iptables -t nat -I prerouting_rule -m mac --mac-source 00:90:4B:B1:5A:75 -j NET

# Remove a user with MAC address of 00:90:4B:B1:5A:75.
iptables -t nat -D prerouting_rule -m mac --mac-source 00:90:4B:B1:5A:75 -j NET

# Add a website (e.g. www.google.com) to walled garden
iptables -t nat -I prerouting_rule -m state --state NEW,ESTABLISHED,RELATED,INVALID -d "www.google.com" -j ACCEPT

# Remove a website (e.g. www.google.com) from walled garden
iptables -t nat -D prerouting_rule -m state --state NEW,ESTABLISHED,RELATED,INVALID -d "www.google.com" -j ACCEPT
