====================================================
        CISCO PACKET TRACER ROUTING LAB
          (RIP | OSPF | EIGRP)
====================================================


===============================
1) BASIC ROUTER CONFIGURATION
===============================

enable
configure terminal
hostname R1

interface gigabitEthernet 0/0
ip address 192.168.1.1 255.255.255.0
no shutdown
exit

interface gigabitEthernet 0/1
ip address 10.0.0.1 255.255.255.0
no shutdown
exit


(Repeat for R2, R3 with different IP addresses)


====================================================
IMPORTANT:
Use ONLY ONE routing protocol at a time.
If switching protocol, remove old one first.
====================================================


===============================
2) RIP CONFIGURATION (Version 2)
===============================

router rip
version 2
no auto-summary
network 192.168.1.0
network 10.0.0.0
exit


Verify RIP:
--------------------------------
show ip route
show ip protocols


Remove RIP:
--------------------------------
no router rip



===============================
3) OSPF CONFIGURATION
===============================

router ospf 1
network 192.168.1.0 0.0.0.255 area 0
network 10.0.0.0 0.0.0.255 area 0
exit


Verify OSPF:
--------------------------------
show ip route
show ip ospf neighbor
show ip protocols


Remove OSPF:
--------------------------------
no router ospf 1



===============================
4) EIGRP CONFIGURATION
===============================

router eigrp 1
no auto-summary
network 192.168.1.0
network 10.0.0.0
exit


Verify EIGRP:
--------------------------------
show ip route
show ip eigrp neighbors
show ip protocols


Remove EIGRP:
--------------------------------
no router eigrp 1



===============================
SAVE CONFIGURATION
===============================

copy running-config startup-config

OR

write memory



===============================
TESTING COMMANDS
===============================

ping 192.168.2.1
tracert 192.168.3.1



====================================================
EXAM NOTES
====================================================

• RIP uses Hop Count (Max 15)
• OSPF uses Cost (Bandwidth based)
• EIGRP uses Bandwidth + Delay
• OSPF requires wildcard mask
• Always verify using: show ip route
• Check neighbors in OSPF and EIGRP
• Ensure interfaces are "no shutdown"

====================================================

====================================================
