#include "rtspsession.h"
#include "common.h"

RtspSession::RtspSession(){
}
RtspSession::~RtspSession(){
}
bool RtspSession::DoSession(int sockfd){
	char req[MAX_REQ_SIZE];
	while(1) {
		if(!GetRequest(req)) {
			break;
		}
		DealRequest(req);
	}
	return true;
}

bool RtspSession::GetRequest(char *req){
	return true;
}

bool RtspSession::DealRequest(char *req){
	return true;
}

bool RtspSession::DoOptions(char *req){
	return true;
}

bool RtspSession::DoDescribe(char *req){
	return true;
}

bool RtspSession::DoSetup(char *req){
	return true;
}

bool RtspSession::DoPlay(char *req){
	return true;
}

bool RtspSession::DoPause(char *req){
	return true;
}

bool RtspSession::DoTeardown(char *req){
	return true;
}

