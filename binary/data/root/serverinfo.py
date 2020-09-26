import app
SERVER_IP = "192.168.1.11"
SERVER_1 = "TurkmmoMyte2"
CLIENT_VERSION = "4"
CH_1_NAME	= "CH1   "
CH_2_NAME	= "CH2   "
CH_3_NAME	= "CH3   "
CH_4_NAME	= "CH4   "
CH_5_NAME	= "CH5   "
CH_6_NAME	= "CH6   "
CH_1 = 13010
CH_2 = 14010
CH_3 = 15010
CH_4 = 16010
CH_5 = 17010
CH_6 = 18010
AUTH = 11055
MARKADDR	= 13010

app.ServerName = None

STATE_NONE = "..."
		
STATE_DICT = {
	0 : "....",
	1 : "NORM",
	2 : "BUSY",
	3 : "FULL"
}

SERVER01_CHANNEL_DICT = {
	1:{'key':11,'name':'CH1','ip':SERVER_IP,'tcp_port':CH_1,'udp_port':CH_1,'state':STATE_NONE,},
	2:{'key':12,'name':'CH2','ip':SERVER_IP,'tcp_port':CH_2,'udp_port':CH_2,'state':STATE_NONE,},
	3:{'key':13,'name':'CH3','ip':SERVER_IP,'tcp_port':CH_3,'udp_port':CH_3,'state':STATE_NONE,},
	4:{'key':14,'name':'CH4','ip':SERVER_IP,'tcp_port':CH_4,'udp_port':CH_4,'state':STATE_NONE,},
#	5:{'key':15,'name':'CH5','ip':SERVER_IP,'tcp_port':CH_5,'udp_port':CH_5,'state':STATE_NONE,},
#	6:{'key':16,'name':'CH6','ip':SERVER_IP,'tcp_port':CH_6,'udp_port':CH_6,'state':STATE_NONE,},
}

REGION_NAME_DICT = {
	0 : "",		
}

REGION_AUTH_SERVER_DICT = {
	0 : {
		1 : { "ip":SERVER_IP, "port":AUTH, },
	}		
}

REGION_DICT = {
	0 : {
		1 : { "name" :SERVER_1, "channel" : SERVER01_CHANNEL_DICT, },
	},
}

MARKADDR_DICT = {
	10 : { "ip" : SERVER_IP, "tcp_port" : MARKADDR, "mark" : "10_0.tga", "symbol_path" : "0", },
}

#DONE
