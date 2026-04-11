


#Server on 1st PC:-

Check IP-    ip a

Allow Port-  sudo ufw allow 9734
             sudo ufw enable 
             sudo ufw status
             
Run Server-  python3 rsa_server.py

#clinet on 2nd PC:-

after check ip in server 1st pc
change it to server pc IP then

Run Client- python3 rsa_client.py


Check python version:-
Window- python --version
Ubuntu- python3 --version
