# User needed : 
#   level3/level3
#   wayne/873md5tridgell

# Put all files at /root
#   rsyncd.conf , rsyncd.secrets
#   level-info, dot.bash_profile
#   flag.txt

cd /root

# Config Rsyncd
mv rsyncd.conf /etc/rsyncd.conf
chmod 644 /etc/rsyncd.conf
mv rsyncd.secrets /etc/rsyncd.secrets
chmod 644 /etc/rsyncd.secrets

# Rsync module
mkdir -p /var/ctf/level3/upload
chown -R wayne:root /var/ctf

# Flag 
mv flag.txt /home/wayne
chown root:wayne /home/wayne/flag.txt
chmod 640 /home/wayne/flag.txt

# Other stuff
echo "" > /etc/motd
chown root:level3 /home/level3/.bash*
chmod 640 /home/level3/.bash*
mv level-info /home/level3
chmod 644 /home/level3/level-info

