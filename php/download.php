<?php



const  CERTIFICATE_ENTRY_OFFSET = 148;
const  PAYLOAD_ALIGNMENT = 8;

$handle = fopen("zerom-client.exe","rb");

$payload = "nimabasldkf";


if (!$handle) return FALSE;
$Header = fread($handle, 64);
var_dump($Header);
if (substr($Header, 0, 2) != 'MZ') return FALSE;
$PEOffset = unpack("V", substr($Header, 60, 4));
if ($PEOffset[1] < 64) return FALSE;
fseek($handle, $PEOffset[1], SEEK_SET);
$Header = fread($handle, 24);
if (substr($Header, 0, 2) != 'PE') return FALSE;
fseek($handle, -16, SEEK_CUR);
var_dump("0x".dechex(ftell($handle)));

fseek($handle, CERTIFICATE_ENTRY_OFFSET-4, SEEK_CUR);
var_dump("0x".dechex(ftell($handle)));
$cert_table_length_offset = ftell($handle);
var_dump(dechex($cert_table_length_offset+4));
$Header = fread($handle,64);
var_dump($Header);
/*echo dechex(unpack("V",substr($Header,0,4))[1])."\r\n";
echo dechex(unpack("V",substr($Header,4,4))[1])."\r\n";
echo dechex(unpack("V",substr($Header,8,4))[1])."\r\n";
echo dechex(unpack("V",substr($Header,12,4))[1])."\r\n";
echo dechex(unpack("V",substr($Header,16,4))[1])."\r\n";*/
$cert_table_offset = unpack("V",substr($Header,0,4))[1];
$cert_table_length = unpack("V",substr($Header,4,4))[1];

var_dump(dechex($cert_table_length[1]));
var_dump(dechex($cert_table_offset[1]));
ftell($handle);

//exit();
$filename = "zerom-client". date("Ymdgis") .".exe";
$newfile = fopen($filename,"wb+");
rewind($handle);
while (!feof($handle)) {
	fwrite($newfile,fread($handle,512));
}

fseek($newfile,0,SEE_END);
$payload_size = strlen($payload);
$padding_size = PAYLOAD_ALIGNMENT - ($payload_size % PAYLOAD_ALIGNMENT);
var_dump("padding_size".$padding_size);

	if ($padding_size > 0)
	{
		for ($i=0; $i<$padding_size; ++$i)
			fputs($newfile,pack("h",0));
	}

fputs($newfile,$payload);

fseek($newfile, $cert_table_length_offset+4, SEEK_SET);
$fixedSize = $cert_table_length+$payload_size+$padding_size;
var_dump(dechex($fixedSize));
$hexdata = pack('V',$fixedSize);
var_dump($hexdata);
fwrite($newfile,$hexdata);

fseek($newfile, $cert_table_offset, SEEK_SET);
fwrite($newfile,$hexdata);

fclose($newfile);

fclose($handle);




?>