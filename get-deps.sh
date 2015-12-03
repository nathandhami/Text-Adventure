# Installs the dependencies and libraries needed
# by the project to your system

# Boost
echo "Installing Boost..."
apt-get install libboost-all-dev
apt-get install aptitude
aptitude search boost
echo "Boost: done."

# SQLite
echo "Installing SQLite..."
apt-get install sqlite3 libsqlite3-dev
echo "SQLite: done."

# GTK+
echo "Installing GTK+..."
apt-get install libgtkmm-3.0-dev
echo "GTK+: done."

# Sphinx
echo "Installing Sphinx..."
apt-get install python3
apt-get install python-sphinx
echo "Sphinx: done."

# Other libs...
