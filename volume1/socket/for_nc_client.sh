for ((i = 0;i < 200; i++))
do
	nc -nvv 127.0.0.1 8000 &
done
