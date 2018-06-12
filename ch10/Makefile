EXAMPLES = MQTTClient_publish.out \
		   MQTTClient_subscribe.out \
		   MQTTClient_publish_async.out \
		   MQTTAsync_publish.out \
		   MQTTAsync_subscribe.out

all : $(EXAMPLES)

CFLAGS = -Wall -W -Wno-unused-parameter 
LIBS = -lpaho-mqtt3c
LIBS_AS = -lpaho-mqtt3as

MQTTClient_publish.out : MQTTClient_publish.c
	gcc $(CFLAGS) $(LIBS) $^ -o $@

MQTTClient_publish_async.out : MQTTClient_publish_async.c
	gcc $(CFLAGS) $(LIBS) $^ -o $@

MQTTClient_subscribe.out : MQTTClient_subscribe.c
	gcc $(CFLAGS) $(LIBS) $^ -o $@

MQTTAsync_publish.out : MQTTAsync_publish.c
	gcc $(CFLAGS) $(LIBS_AS) $^ -o $@

MQTTAsync_subscribe.out : MQTTAsync_subscribe.c
	gcc $(CFLAGS) $(LIBS_AS) $^ -o $@

clean : 
	-rm $(EXAMPLES)

