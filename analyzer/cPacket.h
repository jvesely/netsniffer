#include <QByteArray>
class cPacket{
private:
	QByteArray data;
public:
	cPacket(const char * data_, const int len):data(data_,len){};
	void write();
};