# User needed : 
#   level4/level4
#   lambda/TUf74v6ducre
#   vexillum/UL967R5HH1b8I3
useradd -c 'Lambda User, Love SSH' -m -p 'TUf74v6ducre' -s "/bin/bash" lambda
useradd -c 'Vexillum, Flag My Home' -m -p 'UL967R5HH1b8I3' -s "/bin/bash" vexillum
useradd -c "Level4 User" -m -p "level4" -s '/bin/bash' level4

# Put all files in /root
#   dot.bash_profile, level-info, flag.txt
#   lambda_rsa, lambda_rsa.pub (passphrase : OpenBSDProject)
#   authorized_keys2_vex
#   ssh-control, sshd_config

cd /root

# Basic stuff
echo "" > /etc/motd
rm /home/level4/.profile
chown root:level4 /home/level4/.bash*
chmod 640 /home/level4/.bash*
mv level-info /home/level4
chmod 644 /home/level4/level-info
mv flag.txt /home/vexillum/flag.txt
chown root:vexillum /home/vexillum/flag.txt
chmod 640 /home/vexillum/flag.txt

# SSH + Keychain : lambda
mkdir -p /home/lambda/.ssh
chown -R lambda:lambda /home/lambda/.ssh
chmod 700 /home/lambda/.ssh
mv lambda_rsa /home/lambda/.ssh/id_rsa
mv lambda_rsa.pub /home/lambda/.ssh/id_rsa.pub
chmod 600 /home/lambda/.ssh/id_rsa
chmod 644 /home/lambda/.ssh/id_rsa.pub

# SSH : vexillum
mkdir -p /home/vexillum/.ssh
chown vexillum:vexillum /home/vexillum/.ssh/
chmod 700 /home/vexillum/.ssh
mv authorized_keys2_vex /home/vexillum/.ssh/authorized_keys2
chmod 600 /home/vexillum/.ssh/authorized_keys2

# SSH Global 
mv sshd_config /etc/ssh
mv ssh-control /home
chown root:vexillum /home/ssh-control
chmod 754 /home/ssh-control

# Services
/etc/init.d/ssh restart
