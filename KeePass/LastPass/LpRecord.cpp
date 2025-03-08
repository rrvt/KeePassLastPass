// Password LPentry data


#include "pch.h"
#include "LpRecord.h"
#include "Groups.h"
#include <random>


TCchar* FavOpen     = _T("<Fav~>");
TCchar* FavClose    = _T("</Fav~>\n");
TCchar* ExtraOpen   = _T("<Extra~>");
TCchar* ExtraClose  = _T("</Extra~>\n");
TCchar* ToTpOpen    = _T("<ToTP~>");
TCchar* ToTpClose   = _T("</ToTP~>\n");
TCchar* TimeOpen    = _T("<Time~>");
TCchar* TimeClose   = _T("</Time~>\n");
TCchar* FFLineOpen  = _T("<FFLine~>");
TCchar* FFLineClose = _T("</FFLine~>\n");



LpRecord::~LpRecord() {
  clr(grpName);  clr(url);   clr(title); clr(userName);
  clr(password); clr(notes); clr(fav);   clr(additional);
  }


void LpRecord::clear() {
  grpName.clear();     url.clear();    title.clear();   userName.clear();    password.clear();
  totp.clear();        notes.clear();  fav.clear();     additional.clear();
  }




void LpRecord::setAdditional() {
  parseNote();
  additional  = FavOpen;     additional += fav;     additional += FavClose;
  additional += ExtraOpen;   additional += notes;   additional += ExtraClose;
  additional += ToTpOpen;    additional += totp;    additional += ToTpClose;
  }



static TCchar*   CCType   = _T("NoteType:Credit Card");
static TCchar*   WiFiType = _T("NoteType:Wi-Fi Password");
static TCchar*   DBType   = _T("NoteType:Database");
static TCchar*   AddrType = _T("NoteType:Address");
static TCchar*   BankType = _T("NoteType:Bank Account");
static TCchar*   NameCard = _T("Name on Card:");
static TCchar*   CCNumber = _T("Number:");
static TCchar*   SecCode  = _T("Security Code:");
static TCchar*   PassWD   = _T("Password:");
static TCchar*   EOL      = _T("\n");
static TCchar*   Divider  = _T("<~>");


void LpRecord::parseNote() {
String prefix;
String suffix;

  if (notes.find(CCType)   >= 0) {parseCC();   return;}
  if (notes.find(WiFiType) >= 0) {parseWiFi(); return;}
  if (notes.find(DBType)   >= 0) {parseDB();   return;}
  if (notes.find(AddrType) >= 0) {parseAddr(); return;}
  if (notes.find(BankType) >= 0) {parseBank(); return;}
  }


/*  http://sn,,,,"NoteType:Credit Card
    Language:en-US
    Name on Card:Robert-Reed Van Tuyl
    Type:VISA
    Number:3333333339
    Security Code:999
    Start Date:January,2010
    Expiration Date:June,2024
    Notes:",Citi Card,Finance,0  */

void LpRecord::parseCC() {
String prefix;
String suffix;

  userName += Divider + extract(NameCard, EOL, notes,    prefix, suffix);
  password += Divider + extract(CCNumber, EOL, suffix, prefix, suffix);
  password += Divider + extract(SecCode,  EOL, suffix, prefix, suffix);

  notes = prefix + suffix;
  }


/*
http://sn,,12345678,,"NoteType:Wi-Fi Password
SSID:DIRECT-6E-HP OfficeJet 250
Password:12345678
Connection Type:ESS
Connection Mode:auto
Authentication:WPA2PSK
Encryption:AES
Use 802.1X:false
FIPS Mode:
Key Type:passPhrase
Protected:false
Key Index:
Notes:<?xml version=""1.0""?>
<WLANProfile xmlns=""http://www.microsoft.com/networking/WLAN/profile/v1"">
  <name>DIRECT-6E-HP OfficeJet 250</name>
  <SSIDConfig>
    <SSID>
      <hex>4449524543542D36452D4850204F66666963654A657420323530</hex>
      <name>DIRECT-6E-HP OfficeJet 250</name>
    </SSID>
  </SSIDConfig>
  <connectionType>ESS</connectionType>
  <connectionMode>auto</connectionMode>
  <MSM>
    <security>
      <authEncryption>
        <authentication>WPA2PSK</authentication>
        <encryption>AES</encryption>
        <useOneX>false</useOneX>
      </authEncryption>
      <sharedKey>
        <keyType>passPhrase</keyType>
        <protected>false</protected>
        <keyMaterial>12345678</keyMaterial>
      </sharedKey>
    </security>
  </MSM>
</WLANProfile>
",DIRECT-6E-HP OfficeJet 250,Notes,0
*/

static TCchar* KeyMat  = _T("<keyMaterial>");
static TCchar* EKeyMat = _T("</keyMaterial>");


void LpRecord::parseWiFi() {
String prefix;
String suffix;
String pw;

  password += Divider + extract(PassWD, EOL,     notes,    prefix, suffix);
  password += Divider + extract(KeyMat, EKeyMat, suffix, prefix, suffix);

  notes = prefix + suffix;
  }


/* http://sn,No User Name,XYZERFT,,"NoteType:Database
   Language:en-US
   Type:LastPass
   Hostname:My Host
   Port:73
   Database:Last Pass
   Username:No User Name
   Password:XYZERFT
   SID:4586
   Alias:LP
   Notes:Another Note",LastPass,Maintenance,0  */

void LpRecord::parseDB() {
String prefix;
String suffix;
String pw;

  password += Divider + extract(PassWD, EOL, notes, prefix, suffix);

  notes = prefix + suffix;
  }


/*  http://sn,,,,"NoteType:Address
    Language:en-US
    Title:mr
    First Name:Robert
    Middle Name:Reed
    Last Name:Van Tuyl
    Username:RRVanTuyl
    Gender:m
    Birthday:September,7,1900
    Company:
    Address 1:2222 XRay Drive
    Address 2:
    Address 3:
    City / Town:San Jose
    County:Santa Clara
    State:CA
    Zip / Postal Code:95155-4432
    Country:US
    Timezone:-18:00,1
    Email Address:ssww@wwss.com
    Phone:{""num"":""14089998888"",""ext"":"""",""cc3l"":""USA""}
    Evening Phone:
    Mobile Phone:{""num"":""14087776666"",""ext"":"""",""cc3l"":""USA""}
    Fax:{""num"":""14088886666"",""ext"":"""",""cc3l"":""USA""}
    Notes:This a test
    of an Address Note",Robert R. Van Tuyl,Notes,1   */

static TCchar* BirthDy = _T("Birthday:");


void LpRecord::parseAddr() {
String prefix;
String suffix;

  password = extract(BirthDy, EOL, notes, prefix, suffix);

  notes = prefix + suffix;
  }

/*
http://sn,,,,"NoteType:Bank Account
Language:en-US
Bank Name:My Bank
Account Type:Checking
Routing Number:1234567890
Account Number:0987654321
SWIFT Code:13579
IBAN Number:08642d
Pin:1234
Branch Address:1110 Basic Street
Branch Phone:408.999.7777
Notes:A Note",Bank Account,Finance,0 */

static TCchar* BankName  = _T("Bank Name:");
static TCchar* RoutingNo = _T("Routing Number:");
static TCchar* AccountNo = _T("Account Number:");
static TCchar* SwiftCode = _T("SWIFT Code:");
static TCchar* IbanCode  = _T("IBAN Number:");
static TCchar* PinCode   = _T("Pin:");

void LpRecord::parseBank() {
String prefix;
String suffix;

  userName += Divider + extract(BankName,  EOL, notes,    prefix, suffix);
  password += Divider + extract(RoutingNo, EOL, suffix, prefix, suffix);
  password += Divider + extract(AccountNo, EOL, suffix, prefix, suffix);
  password += Divider + extract(SwiftCode, EOL, suffix, prefix, suffix);
  password += Divider + extract(IbanCode,  EOL, suffix, prefix, suffix);
  password += Divider + extract(PinCode,   EOL, suffix, prefix, suffix);

  notes = prefix + suffix;
  }


String LpRecord::extract(TCchar* beg, TCchar* end, String& src, String& prefix, String& suffix) {
int    bPos = src.find(beg) + _tcslen(beg);
int    ePos = src.find(end, bPos);       if (bPos < 0 || ePos < 0) {suffix = src; return _T("");}
String s   = src.substr(bPos, ePos-bPos);

  prefix += src.substr(0, bPos);   suffix = src.substr(ePos);   return s;
  }


/*
  USHORT shYear; ///< Year. 2004 means 2004.
  BYTE btMonth;  ///< Month. Ranges from 1 = Jan to 12 = Dec.
  BYTE btDay;    ///< Day. The first day is 1.
  BYTE btHour;   ///< Hour. Begins with hour 0, maximum value is 23.
  BYTE btMinute; ///< Minutes. Begins at 0, maximum value is 59.
  BYTE btSecond; ///< Seconds. Begins at 0, maximum value is 59.
*/

LpRecord& LpRecord::operator= (KpRecord& rcd) {
String s = rcd.pszAdditional;

  clear();

  grpName    = groups.getName(rcd.uGroupId);
  title      = rcd.pszTitle;
  url        = rcd.pszURL;
  userName   = rcd.pszUserName;
  password   = rcd.pszPassword;
  additional = rcd.pszAdditional;

  restoreNote();

  set(FavOpen,   FavClose,   fav);
  set(ExtraOpen, ExtraClose, notes);
  set(ToTpOpen,  ToTpClose,  totp);
  set(TimeOpen,  TimeClose,  timeStamp);

  if (timeStamp.isEmpty()) {
    PW_TIME& tm = rcd.tCreation;

    timeStamp.format(_T("%02i/%02i/%02i %02i:%02i:%02i"),
                            tm.btMonth, tm.btDay, tm.shYear % 100, tm.btHour, tm.btMinute, tm.btSecond);
    }

  return *this;
  }


void LpRecord::restoreNote() {
  if (additional.find(CCType)   >= 0) {restoreCC();   return;}
  if (additional.find(WiFiType) >= 0) {restoreWiFi(); return;}
  if (additional.find(DBType)   >= 0) {restoreDB();   return;}
  if (additional.find(AddrType) >= 0) {restoreAddr(); return;}
  if (additional.find(BankType) >= 0) {restoreBank(); return;}
  }


/*  http://sn,,,,"NoteType:Credit Card
    Language:en-US
    Name on Card:Robert-Reed Van Tuyl
    Type:VISA
    Number:3333333339
    Security Code:999
    Start Date:January,2010
    Expiration Date:June,2024
    Notes:",Citi Card,Finance,0  */


void LpRecord::restoreCC() {
String item;

  if (nextItem(userName, item)) insert(NameCard, item);
  if (nextItem(password, item)) insert(CCNumber, item);
  if (nextItem(password, item)) insert(SecCode,  item);
  }


/*
http://sn,,12345678,,"NoteType:Wi-Fi Password
SSID:DIRECT-6E-HP OfficeJet 250
Password:12345678
Connection Type:ESS
Connection Mode:auto
Authentication:WPA2PSK
Encryption:AES
Use 802.1X:false
FIPS Mode:
Key Type:passPhrase
Protected:false
Key Index:
Notes:<?xml version=""1.0""?>
<WLANProfile xmlns=""http://www.microsoft.com/networking/WLAN/profile/v1"">
  <name>DIRECT-6E-HP OfficeJet 250</name>
  <SSIDConfig>
    <SSID>
      <hex>4449524543542D36452D4850204F66666963654A657420323530</hex>
      <name>DIRECT-6E-HP OfficeJet 250</name>
    </SSID>
  </SSIDConfig>
  <connectionType>ESS</connectionType>
  <connectionMode>auto</connectionMode>
  <MSM>
    <security>
      <authEncryption>
        <authentication>WPA2PSK</authentication>
        <encryption>AES</encryption>
        <useOneX>false</useOneX>
      </authEncryption>
      <sharedKey>
        <keyType>passPhrase</keyType>
        <protected>false</protected>
        <keyMaterial>12345678</keyMaterial>
      </sharedKey>
    </security>
  </MSM>
</WLANProfile>
",DIRECT-6E-HP OfficeJet 250,Notes,0
*/

void LpRecord::restoreWiFi() {
String item;

  if (nextItem(password, item)) insert(PassWD, item);
  if (nextItem(password, item)) insert(KeyMat,  item);
  }


void LpRecord::restoreDB() {
String item;

  if (nextItem(password, item)) insert(PassWD, item);
  }


void LpRecord::restoreAddr() {
String item;

  if (nextItem(password, item)) insert(BirthDy, item);
  }


void LpRecord::restoreBank() {
String item;

  if (nextItem(userName, item)) insert(BankName,  item);
  if (nextItem(password, item)) insert(RoutingNo, item);
  if (nextItem(password, item)) insert(AccountNo, item);
  if (nextItem(password, item)) insert(SwiftCode, item);
  if (nextItem(password, item)) insert(IbanCode,  item);
  if (nextItem(password, item)) insert(PinCode,   item);
  }


bool LpRecord::nextItem(String& src, String& item) {
int    pos    = src.find(Divider);          if (pos < 0) return false;
int    end    = src.find(Divider, pos+1);   if (end < 0) end = src.length();
String s      = src.substr(pos, end - pos);
String prefix = src.substr(0,   pos);
String suffix = src.substr(end);

  src = prefix + suffix;

  item = s.substr(_tcslen(Divider));   return true;
  }


void LpRecord::insert(TCchar* beg, String& stg) {
int pos       = additional.find(beg);   if (pos < 0) return;   pos += _tcslen(beg);
String prefix = additional.substr(0, pos);
String suffix = additional.substr(pos);

  additional = prefix + stg + suffix;
  }



void LpRecord::set(TCchar* opn, TCchar* cls, String& dst) {
int opnPos = additional.find(opn);
int endPos = additional.find(cls);
int count;

  dst.clear();

  if (opnPos < 0 || endPos < 0) return;

  opnPos += _tcslen(opn);   count = endPos - opnPos;   if (count <= 0) return;

  dst = additional.substr(opnPos, count);
  }


void LpRecord::clr(String& s) {
uniform_int_distribution<int> rand(32, 127);
random_device                 rd;
mt19937                       gen(rd());
int n = s.length();
int i;
TCHAR ch = s[0] - (TCHAR) n;

  for (i = 0; i < n; i++) {
    ch = (char) rand(gen);
    s[i] = ch++;
    }
  }



////-------------------------

#if 0
void LpRecord::updateNotes() {
  if (notes.find(CCType)   >= 0) {updateCC();    return;}
  if (notes.find(WiFiType) >= 0) {updateWiFi();  return;}
  if (notes.find(DBType)   >= 0) {updateDB();    return;}
  if (notes.find(AddrType) >= 0) {updateAddr();  return;}
  if (notes.find(BankType) >= 0) {updateBank();  return;}
  }


/*
  userName += Divider + extract(NameCard, EOL, stg,    prefix, suffix);
  password += Divider + extract(CCNumber, EOL, suffix, prefix, suffix);
  password += Divider + extract(SecCode,  EOL, suffix, prefix, suffix);
*/

void LpRecord::updateCC() {
String s;

  if (!nextItem(userName, s)) return;   update(NameCard, s);
  if (!nextItem(password, s)) return;   update(CCNumber, s);
  if (!nextItem(password, s)) return;   update(SecCode,  s);
  }


/*

  pw = extract(PassWD, EOL, stg, prefix, suffix);

  if (password.isEmpty() && !pw.isEmpty()) password = pw;
  pw = extract(KeyMat, EKeyMat, suffix, prefix, suffix);


  if (password.isEmpty() && !pw.isEmpty()) password = pw;
*/

void LpRecord::updateWiFi() {

  update(PassWD, password);
  update(KeyMat, password);
  }


/*
  pw = extract(PassWD, EOL, stg, prefix, suffix);

  if (password.isEmpty() && !pw.isEmpty()) password = pw;
*/

void LpRecord::updateDB() {
  update(PassWD, password);
  }


/*
  password = extract(BirthDy, EOL, stg, prefix, suffix);
*/

void LpRecord::updateAddr() {
  update(BirthDy, password);    password.clear();
  }


/*
  userName =            extract(BankName,  EOL, stg,    prefix, suffix);
  password += Divider + extract(RoutingNo, EOL, suffix, prefix, suffix);
  password += Divider + extract(AccountNo, EOL, suffix, prefix, suffix);
  password += Divider + extract(SwiftCode, EOL, suffix, prefix, suffix);
  password += Divider + extract(IbanCode,  EOL, suffix, prefix, suffix);
  password += Divider + extract(PinCode,   EOL, suffix, prefix, suffix);
*/

void LpRecord::updateBank() {
String s;

  update(BankName, userName);   userName.clear();
  if (nextItem(password, s)) update(RoutingNo, s);
  if (nextItem(password, s)) update(AccountNo, s);
  if (nextItem(password, s)) update(SwiftCode, s);
  if (nextItem(password, s)) update(IbanCode,  s);
  if (nextItem(password, s)) update(PinCode,   s);
  }
#endif

