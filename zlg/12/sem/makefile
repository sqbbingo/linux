obj1 = sem_wait 
ojb2 = sem_post
all:$(obj1) $(obj2)

sem_wait:sem_wait.c
	gcc sem_wait.c -o sem_wait -pthread -lrt

sem_post:sem_post.c
	gcc sem_post.c -o sem_post -pthread -lrt

.PHONY:clean
clean:
	-rm $(obj1) $(obj2)
