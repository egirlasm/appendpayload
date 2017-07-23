void fixPayload() {
	const int CERTIFICATE_ENTRY_OFFSET = 148;
	const int PAYLOAD_ALIGNMENT = 8;
	FILE * f;
	fopen_s(&f, g_input_file.c_str(), "rb");
	DWORD cert_table_offset = 0;
	DWORD cert_table_length = 0;


	while (fgetc(f) != EOF)
	{
		char c1 = fgetc(f);
		if (c1 == 'P')
		{
			char c2 = fgetc(f);
			if (c2 == 'E')
			{
				char c3 = fgetc(f);
				if (c3 == '\0')
				{
					char c4 = fgetc(f);
					if (c4 == '\0')
						break;
				}
			}
		}
	}
	fseek(f, CERTIFICATE_ENTRY_OFFSET, SEEK_CUR);
	fgets(reinterpret_cast<char*>(&cert_table_offset), 4, f);

	fseek(f, 1, SEEK_CUR);
	int cert_table_length_offset = ftell(f);
	fgets(reinterpret_cast<char*>(&cert_table_length), 4, f);

	fseek(f, cert_table_offset, SEEK_SET);

	DWORD cert_table_length2 = 0;
	fgets(reinterpret_cast<char*>(&cert_table_length2), 4, f);
	if (cert_table_length == cert_table_length2) {
		printf("its ok");
	}


	const int buf_size = 8192;
	char* buffer = new char[buf_size];	// known leak!!

	fseek(f, 0, SEEK_SET);


	char buf[BUFSIZ];
	size_t size;

	FILE * out;
	time_t current_time = time(NULL);
	struct tm tm;
	localtime_s(&tm, &current_time);

	char szFileName[255] = { 0 };
	sprintf_s(szFileName, "zerom-client%d%d%d%d%d.exe", tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

	fopen_s(&out, szFileName, "wb+");
	while (size = fread(buf, 1, BUFSIZ, f)) {
		fwrite(buf, 1, size, out);
	}



	fseek(out, 0, SEEK_END);
	fputc(3, out);
	fputs("caonima", out);

	fseek(out, cert_table_length_offset, SEEK_SET);
	//cert_table_length = 0;
	//fgets(reinterpret_cast<char*>(&cert_table_length), 4, f);
	cert_table_length = cert_table_length + 8;
	//fgets(reinterpret_cast<char*>(&cert_table_length), 4, f);
	int written = fwrite(reinterpret_cast<char*>(&cert_table_length), 1, 4, out);
	printf("written %d ", written);
	fseek(out, cert_table_offset, SEEK_SET);

	fwrite(reinterpret_cast<char*>(&cert_table_length), 1, 4, out);
	//fputs(reinterpret_cast<char*>(&cert_table_length), out);

	fclose(out);

	fclose(f);

}