#pragma once

#define MAXSURVIVORUSER 4
#define MAXKILLERUSER 1

#define MAXROOMUSER 5

#define TRYCONNECTTIME 5.f
#define PINGTIME 0.25f

#define BROADCASTTIME 0.05f
#define SYNCROTIME   0.3333333f

#define LOBBYROOM -1

#define MAXLOGSTRING 256 

#define MAXROOMCOUNT 6					// ���� �� �����ϴ� �� ���� : �׽�Ʈ�� ��(0��) + ���� ���� ��(1�� ~ 5��)
#define MAXACCESSROOMCOUNT 5			// ���� ���ӷ밹��


#define MAXMAGICCIRCLE 40

#define TIMING(Acc,Delta,Time) ((Acc < Time) && ((Acc + Delta) >= Time) )