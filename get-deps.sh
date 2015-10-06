# Installs the dependencies and libraries needed
# by the project to your system

# Boost
echo "Installing Boost..."
apt-get install libboost-all-dev
apt-get install aptitude
aptitude search boost
echo "Boost: done."

# Other libs...