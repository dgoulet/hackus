# At root@ctff02 in /root
# Put every file in /root

# For this level, we need 4 users :
#   - alice, bob, charlie : NO PASSWORD ==> CANNOT LOG IN
#   - level2:level2

# File needed :
#   alice.py bob.py charlie.py 
#   launch-alice cron-alice cron-bob cron-charlie 
#   level2.sql level-info dot.bash_profile

# Moving file in each home user
mv alice.py /home/alice/
mv launch-alice /home/alice/
mv bob.py /home/bob/
mv charlie.py /home/charlie/

# Alice Setup 
chown alice:alice /home/alice/alice.py
chown alice:alice /home/alice/launch-alice
chmod 600 /home/alice/alice.py
chmod 700 /home/alice/launch-alice

# Bob Setup
chown bob:bob /home/bob/bob.py
chmod 600 /home/bob/bob.py

# Charlie Setup
chown charlie:charlie /home/charlie/charlie.py
chmod 644 /home/charlie/charlie.py

# Setting file flag
touch /lib/tls/i686/cmov/FlAg_l3v3l_Two.txt
chmod 646 /lib/tls/i686/cmov/FlAg_l3v3l_Two.txt
mkdir -p /var/cache/l2/sub/src
echo "Yesterday it worked Today it is not working Windows is like that" > /var/cache/l2/sub/src/blowfish_me.blf

# Setting cron for user
crontab -u alice cron-alice
crontab -u bob cron-bob
crontab -u charlie cron-charlie

# Other stuff
echo "" > /etc/motd
chown root:level2 /home/level2/.bash*
chmod 640 /home/level2/.bash*
mv level-info /home/level2
chmod 644 /home/level2/level-info
# Sudo policy for tcpdump
echo "level2  ctff02=/usr/sbin/tcpdump" >> /etc/sudoers

# MySQL will ask for root password (clipperz)
mysql -u root -p < level2.sql
