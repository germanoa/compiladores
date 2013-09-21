
hexdump $1 > /tmp/a
hexdump $2 > /tmp/b
diff /tmp/a /tmp/b
