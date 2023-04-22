#ifndef D3B6D082_6AD6_40F7_8824_78611958F839
#define D3B6D082_6AD6_40F7_8824_78611958F839

/*

inline std::string bytes2hex(const uint8_t *in, int size)
{
	std::string out;
	char temp[8];
	for (int i = 0; i < size; ++i) {
		snprintf(temp, sizeof(temp), "%02hhx", in[i]);
		out += temp;
	}
	return out;
}

std::string hex = tc::uchar2string((const uchar *)buffer, size);
	std::string hexAsText;
	for(int i=0; i < hex.length(); i+=2)
	{
			std::string byte = hex.substr(i,2);
			char chr = (char) (int)strtol(byte.c_str(), NULL, 16);
			hexAsText.push_back(chr);
	}

	std::vector<std::string> strings;
	std::istringstream f(hexAsText);
	std::string s;
	while (getline(f, s, ',')) {
			strings.push_back(s);
	}

	if (strings[4] == "H0") {
		SysTime now(true);
		std::string answer = fmt::format("*CMDS,OM,862205055040770,000000000000,L0,0,0,{}#", now.timestamp());
		unsigned char first[2];
		first[0] = 0xFF;
		first[1] = 0xFF;
		unsigned char second[answer.length()];
		memcpy(second, answer.data(), answer.length());

		unsigned char out[answer.length() + 2];
		memcpy(out, first, 2);
		memcpy(out + 2, second, answer.length());

		send(out, answer.length() + 2, true);

		LG_NFO(this->logger(), "L0 out: {}", tc::uchar2string(out, answer.length() + 2));
	}

	LG_NFO(this->logger(), "hexAsText: {}", hexAsText);
	std::string answer = fmt::format("*CMDS,OM,862205055040770,000000000000,Re,{}#", strings[4]);
	unsigned char first[2];
	first[0] = 0xFF;
	first[1] = 0xFF;
	unsigned char second[answer.length()];
	memcpy(second, answer.data(), answer.length());

	unsigned char out[answer.length() + 2];
	memcpy(out, first, 2);
	memcpy(out + 2, second, answer.length());

	send(out, answer.length() + 2, true);

	LG_NFO(this->logger(), "out: {}", tc::uchar2string(out, answer.length() + 2));








	/*result_t res = RES_OK;

	if (type == teltonika::Action::payload) {
		res = handlePayload(buffer, size);
	}

	if (type == teltonika::Action::imei) {
		res = handleImei(buffer, size);
	}

	if (type == teltonika::Action::command) {
		res = server()->handleCommand(buffer, size);
	}

	if (type == teltonika::Action::request) {
		res = server()->handleRequest(buffer, size, id());
	}

	if (type == teltonika::Action::standby) {
		res = handleStandby(buffer, size);
	}

	if (type == teltonika::Action::unknown && iBufferIncomplete != nullptr) {
		res = handleIncomplete(buffer, size);
	}

	if (res != RES_OK) {
		LG_DBG(this->logger(), "[{}] Unable to handle data buffer[{}]", iImei, size);
	}*/


*/

#endif /* D3B6D082_6AD6_40F7_8824_78611958F839 */
