<?php
// lang.jp.inc
//
// $Id: lang.jp.inc,v 1.2 2001/09/06 13:49:57 mb Exp $
//
// japanese language definition file
// by Swen Veckes <s.veckes@de.kddi.com>
//
// if you have a translation to another language, please contact
// me --> Matthias Blaser <mb@adfinis.ch>


define("PQL_LANG","jp");

// welcome texts (home.php)
define("PQL_WELCOME", "æ¤±» ");
define("PQL_DESCRIPTION","QMail-LDAP adminstration !!");

// general texts
define("PQL_SAVE", "Û¶");
define("PQL_OPTIONS", "IvV");
define("PQL_EMAIL", "dq[");
define("PQL_ACTIONS", "$B%\"%/%7%g%s(B");

define("PQL_SURE", "{Éæëµ¢Å·©");
define("PQL_YES", "Í¢");
define("PQL_NO", "¢¢¦");
define("PQL_ADD", "ì¬");

define("PQL_DOCUMENTATION", "hLe[V");
define("PQL_SHOW_CONFIG", "Show phpQLAdmin configuration"); // to translate
define("PQL_TEST_LDAP", "Test LDAP-Connection"); // to translate

// error messages
define("PQL_INVALID", "³øÅ·");
define("PQL_MISSING", "©½èÜ¹ñ");
define("PQL_EXISTS", "·ÅÉ¶ÝµÄ¢Ü·");
define("PQL_FAILED_RETYPE", "ÄüÍª³µ­ èÜ¹ñ");
define("PQL_NO_OBJECT", "®«ªÝÂ©èÜ¹ñÅµ½");
define("PQL_GREATER_ZERO", "0$B$h$jBg$-$/$J$1$l$P$J$i$J$$(B");

// user/domain/address things
define("PQL_USER", "[U[");
define("PQL_USER_ID", "[U[¼");
define("PQL_USER_DATA", "[U[f[^");
define("PQL_USER_DATA_SURNAME", "¼O");
define("PQL_USER_DATA_NAME", "¼");
define("PQL_USER_REGISTRED","o^ÏÝ[U[");
define("PQL_USER_NEW", "VK[U[Ìo^");
define("PQL_USER_DELETE", "[U[Ìí");
define("PQL_USER_EDIT", "[U[f[^ÌÏX");
define("PQL_USER_EXIST", "[U[ %user% ·ÅÉ¶ÝµÄ¢Ü·");
define("PQL_USER_NONE", "[U[Ío^³êÄ¢Ü¹ñ");
define("PQL_USER_MAILBOXPROPERTIES", "[{bNXÌvpeB");
define("PQL_USER_ADD_TITLE", "%domain% Ö[U[Ìì¬");
define("PQL_USER_ADD_HELP1", "¼Æ¼OÌgÝí¹ÍhCÌÅBêÌàÌÅÈ¯êÎÈèÜ¹ñBiá@cº Àqj[U[¼Í·×ÄÌVXeÌÅBêÌàÌÅÈ¯êÎÈèÜ¹ñB");
define("PQL_USER_ADD_HELP2", "dq[AhXÆ[U[¼Í©®IÉp¬¶ÉÏ·³êÜ·B");
define("PQL_USER_ADD_OK", "[U[Ío^³êÜµ½");
define("PQL_USER_ADD_FAILED", "[U[o^É¸sµÜµ½");
define("PQL_USER_ACCOUNT_PROPERTIES", "AJEgÌvpeB");
define("PQL_USER_ACCOUNT_PROPERTIES_MORE", "AJEgvpeBÌÇÁ");
define("PQL_USER_ACCOUNT_TYPE", "^Cv");
define("PQL_USER_DEL_TITLE", "%domain% ©ç %user% íµÜ·");
define("PQL_USER_DEL_OK", "[U[Íí³êÜµ½");
define("PQL_USER_DEL_FAILED", "[U[íÉ¸sµÜµ½");
define("PQL_USER_DEL_ATTRIBUTE_TITLE", "®« (%value%) ðíµÜ·");
define("PQL_USER_DEL_ATTRIBUTE_OK", "®«Íí³êÜµ½");
define("PQL_USER_DEL_ATTRIBUTE_FAILED", "®«ÌíÉ¸sµÜµ½");
define("PQL_USER_DELIVERYPROPERTIES","Advanced delivery properties"); // to translate

define("PQL_DNS_TITLE", "DNS îñ");
define("PQL_DNS_MAILHOST", "[zXg");
define("PQL_DNS_PRIORITY", "Dæ");
define("PQL_DNS_NONE", "±ÌhCÍ DNS Éo^³êÜ¹ñ");

define("PQL_DOMAIN_CHANGE_ATTRIBUTE_TITLE", "±ÌhCÌ·×ÄÌ[U[lðÏXµÜ·");
define("PQL_DOMAIN_CHANGE_ATTRIBUTE", "®«");
define("PQL_DOMAIN_INVALID", "³øÈhC¼Å·BhC¼F domain.tld (á nihon.com)");
define("PQL_DOMAIN_EXISTS", "±ÌhCÍ·ÅÉ¶ÝµÄ¢Ü·");

define("PQL_DOMAIN_ADD", "hCÌì¬");
define("PQL_DOMAIN_ADD_OK", "hC %domain% Íì¬³êÜµ½B<br> Qmail ^ Control@fBNg[ Ì rcpthosts Ælocals files Éè®ÅÇÁµÄ­¾³¢");
define("PQL_DOMAIN_ADD_FAILED", "hCì¬É¸sµÜµ½");
define("PQL_DOMAIN_DEL", "hCÌí");
define("PQL_DOMAIN_DEL_TITLE", "hC %domain% Ìí");
define("PQL_DOMAIN_DEL_WARNING", "<b>AÓ:</b> hCðí·éÆ»êÉÜÜêé·×ÄÌ[U[Í¤Éí³êÜ·");
define("PQL_DOMAIN_DEL_OK", "hCÍí³êÜµ½");
define("PQL_DOMAIN_DEL_FAILED", "hCíÉ¸sµÜµ½");

define("PQL_ADDRESS_REGISTRED", "o^ÏÝAhX");
define("PQL_ADDRESS_TYPE", "^Cv");

define("PQL_SENDMAIL", "eXg[ðMµÜ·");
define("PQL_SENDMAIL_OK", "[Í %email% l ÄÉMµÜµ½");
define("PQL_SENDMAIL_FAILED", "[MÉ¸sµÜµ½");


// deliverymode texts
// (normal, forwardonly, nombox, localdelivery, reply, echo)
define("PQL_LDAP_DELIVERYMODE_TITLE","z[h");
define("PQL_LDAP_DELIVERYMODE_MODE","[h");
define("PQL_LDAP_DELIVERYMODE_NORMAL","Ê");
define("PQL_LDAP_DELIVERYMODE_FORWARDONLY","]ÌÝ");
define("PQL_LDAP_DELIVERYMODE_NOMBOX","[{bNXÍ èÜ¹ñ");
define("PQL_LDAP_DELIVERYMODE_LOCALDELIVERY","Û¶");
define("PQL_LDAP_DELIVERYMODE_REPLY","©®Ô");
define("PQL_LDAP_DELIVERYMODE_REPLY_SEND","©®IÉÔ");
define("PQL_LDAP_DELIVERYMODE_REPLY_TEXT","Ô¶");
define("PQL_LDAP_DELIVERYMODE_ECHO","R\[ÖGR[ (tricky)");
define("PQL_LDAP_DELIVERYMODE_CHANGE","vpeBÌÏX");
define("PQL_LDAP_DELIVERYMODE_CHANGE_OK","z[hÍÏX³êÜµ½");
define("PQL_LDAP_DELIVERYMODE_CHANGE_FAILED","z[hÌÏXÉ¸sµÜµ½");
define("PQL_LDAP_DELIVERYMODE_NULL","WÌvpeB");
define("PQL_LDAP_DELIVERYMODE_PROFILE", "vt@C");
define("PQL_LDAP_DELIVERYMODE_PROFILE_LOCAL", "Ê (Mailbox, POP-AJEg)");
define("PQL_LDAP_DELIVERYMODE_PROFILE_FORWARD", "] (]ÌÝ, [{bNXÍ èÜ¹ñ)");

// accountstatus texts
// (active, nopop, disable)
define("PQL_LDAP_ACCOUNTSTATUS_TITLE","AJEgÌóÔ");
define("PQL_LDAP_ACCOUNTSTATUS_STATUS","óÔ");
define("PQL_LDAP_ACCOUNTSTATUS_ACTIVE","ANeBu");
define("PQL_LDAP_ACCOUNTSTATUS_NOPOP","POP ðbN");
define("PQL_LDAP_ACCOUNTSTATUS_DISABLE","bN");
define("PQL_LDAP_ACCOUNTSTATUS_CHANGE","óÔÌÝè");
define("PQL_LDAP_ACCOUNTSTATUS_CHANGE_OK","óÔð %status% ÉÝè");
define("PQL_LDAP_ACCOUNTSTATUS_CHANGE_FAILED","óÔÌÝèÉ¸sµÜµ½");
define("PQL_LDAP_ACCOUNTSTATUS_CHANGE_ACTIVE","ANeBuÉµÜ·");
define("PQL_LDAP_ACCOUNTSTATUS_CHANGE_NOPOP","POP AJEgðbN");
define("PQL_LDAP_ACCOUNTSTATUS_CHANGE_DISABLE","bN");

// mail texts
define("PQL_LDAP_MAIL_TITLE","CAhX");
define("PQL_LDAP_MAIL_CHANGE","CAhXÌÏX");
define("PQL_LDAP_MAIL_HELP", "dq[AhXÍhCðÊµÄBêÌàÌÅÈ¯êÎÈèÜ¹ñ");
define("PQL_LDAP_MAIL_CHANGE_OK", "CAhXð %old% ©ç %new% ÉÏXµÜµ½");
define("PQL_LDAP_MAIL_CHANGE_FAILED", "CAhXÌÏXÉ¸sµÜµ½");
define("PQL_LDAP_MAIL_UNCHANGED", "CAhXÍÏX³êÜ¹ñÅµ½");

// mailforwardingaddress texts
define("PQL_LDAP_MAILFORWARDINGADDRESS_TITLE","]");
define("PQL_LDAP_MAILFORWARDINGADDRESS_OTHER", "±Ì[U[É¼ÌAJEg©ç]·é");
define("PQL_LDAP_MAILFORWARDINGADDRESS_NEW","VK]AhXÌì¬");
define("PQL_LDAP_MAILFORWARDINGADDRESS_ADD_HELP", "[U[Ú×y[WÅ³çÉ]AhXðÇÁÅ«Ü·");
define("PQL_LDAP_MAILFORWARDINGADDRESS_ADD_OK", "]ð %mail% Éì¬µÜµ½");
define("PQL_LDAP_MAILFORWARDINGADDRESS_ADD_FAILED", "]ì¬É¸sµÜµ½");
define("PQL_LDAP_MAILFORWARDINGADDRESS_NONE"," èÜ¹ñ");
define("PQL_LDAP_MAILFORWARDINGADDRESS_CHANGE", "]AhXÌÏX");
define("PQL_LDAP_MAILFORWARDINGADDRESS_CHANGE_OK", "]AhXð %old% ©ç %new% ÉÏXµÜµ½");
define("PQL_LDAP_MAILFORWARDINGADDRESS_CHANGE_FAILED", "]AhXÌÏXÉ¸sµÜµ½");
define("PQL_LDAP_MAILFORWARDINGADDRESS_UNCHANGED", "]AhXÍÏX³êÜ¹ñÅµ½");

// mailalternateaddress texts
define("PQL_LDAP_MAILALTERNATEADDRESS_TITLE","¼AhX");
define("PQL_LDAP_MAILALTERNATEADDRESS_NEW","VKA¼AhXÌÇÁ");
define("PQL_LDAP_MAILALTERNATEADDRESS_DEL", "¼AhXÌí");
define("PQL_LDAP_MAILALTERNATEADDRESS_HELP", "¼AhXÍhCðÊµÄBêÅÈ¯êÎÈèÜ¹ñ");
define("PQL_LDAP_MAILALTERNATEADDRESS_ADD_OK", "¼AhXð %mail% Éì¬µÜµ½");
define("PQL_LDAP_MAILALTERNATEADDRESS_ADD_FAILED", "¼AhXì¬É¸sµÜµ½");
define("PQL_LDAP_MAILALTERNATEADDRESS_CHANGE", "¼AhXÌÏX");
define("PQL_LDAP_MAILALTERNATEADDRESS_CHANGE_OK", "¼AhXð %old% ©ç %new% ÉÏXµÜµ½");
define("PQL_LDAP_MAILALTERNATEADDRESS_CHANGE_FAILED", "¼AhXÌÏXÉ¸sµÜµ½");
define("PQL_LDAP_MAILALTERNATEADDRESS_UNCHANGED","¼AhXÍÏX³êÜ¹ñÅµ½");

// userpassword texts
define("PQL_LDAP_USERPASSWORD_TITLE", "pX[h");
define("PQL_LDAP_USERPASSWORD_NONE", " èÜ¹ñ");
define("PQL_LDAP_USERPASSWORD_ENCRYPTED", "R[h(Ã)Éu··é");
define("PQL_LDAP_USERPASSWORD_NEW", "pX[hÌÝè");
define("PQL_LDAP_USERPASSWORD_RETYPE", "pX[h(ÄüÍ)");
define("PQL_LDAP_USERPASSWORD_HELP", "pX[hÍAt@xbgÜ½Í¾¯ªg¦Ü·BÁê¶Íg¦Ü¹ñ");
define("PQL_LDAP_USERPASSWORD_CHANGE_OK", "pX[hÍÏX³êÜµ½");
define("PQL_LDAP_USERPASSWORD_CHANGE_FAILED", "pX[hÌÏXÉ¸sµÜµ½");

// uid texts
define("PQL_LDAP_UID_TITLE", "[U[¼");
define("PQL_LDAP_UID_NONE", " èÜ¹ñ");
define("PQL_LDAP_UID_HELP", "[U[¼ÍAt@xbgAÜ½ÍÌLªg¦Ü·B &quot;@&quot;, &quot;.&quot;, &quot;_&quot;, &quot;-&quot; bestehen. ³çÉA[U[¼ÍVXeðÊµÄBêÌàÌÅÈ¯êÎÈèÜ¹ñB");
define("PQL_LDAP_UID_HELP_SHORT", "AAt@xbgAÁê¶ &quot;@&quot;, &quot;.&quot;, &quot;_&quot;, &quot;-&quot;");
define("PQL_LDAP_UID_CHANGE", "[U[¼ÌÏX");
define("PQL_LDAP_UID_CHANGE_OK", "[U[¼ð %old% ©ç %new% ÖÏXµÜµ½");
define("PQL_LDAP_UID_CHANGE_FAILED", "[U[¼ÏXÉ¸sµÜµ½");
define("PQL_LDAP_UID_UNCHANGED","[U[¼ÍÏX³êÜ¹ñÅµ½");

// mailquota texts
define("PQL_LDAP_MAILQUOTA_TITLE", "eÊ");
define("PQL_LDAP_MAILQUOTA_NONE", "W");
define("PQL_LDAP_MAILQUOTA_ECHO_MAXSIZE", "Åå %maxsize% KB");
define("PQL_LDAP_MAILQUOTA_ECHO_MAXSIZE_UNLIMITED", "³§ÀÌTCY");
define("PQL_LDAP_MAILQUOTA_ECHO_MAXMAILS", "Åå %maxmails% dq[");
define("PQL_LDAP_MAILQUOTA_ECHO_MAXMAILS_UNLIMITED", "³§Àdq[Ì");
define("PQL_LDAP_MAILQUOTA_HELP", "WÅÍ qmail/LDAP ª configuration file ©çeÊð¾Ü·");
define("PQL_LDAP_MAILQUOTA_NONESELECTED", "ÅáÐÆÂÌeÊ^CvðIñÅ­¾³¢");
define("PQL_LDAP_MAILQUOTA_MAXSIZE_TITLE", "Åå [{bNXÌTCY");
define("PQL_LDAP_MAILQUOTA_MAXMAILS_TITLE", "Åå dq[Ì");
define("PQL_LDAP_MAILQUOTA_DEFAULT", "W");
define("PQL_LDAP_MAILQUOTA_USERDEFINED", "[U[Íè`³êÜµ½");
define("PQL_LDAP_MAILQUOTA_CHANGE", "eÊÌÏX");
define("PQL_LDAP_MAILQUOTA_CHANGE_OK", "eÊð %quota% ÉÏXµÜµ½");
define("PQL_LDAP_MAILQUOTA_CHANGE_FAILED", "eÊÌÏXÉ¸sµÜµ½");
define("PQL_LDAP_MAILQUOTA_UNCHANGED","eÊÌÏX³êÜ¹ñÅµ½");

// mailhost texts
define("PQL_LDAP_MAILHOST_TITLE", "[T[o[");
define("PQL_LDAP_MAILHOST_FQDN", "AhX(FQDN)");
define("PQL_LDAP_MAILHOST_NONE", " èÜ¹ñ");
define("PQL_LDAP_MAILHOST_CHANGE", "[T[o[ÌÏX");
define("PQL_LDAP_MAILHOST_CHANGE_OK", "[T[o[ð %mailhost% ÉÏXµÜµ½");
define("PQL_LDAP_MAILHOST_CHANGE_FAILED", "[T[oÌÏXÉ¸sµÜµ½");
define("PQL_LDAP_MAILHOST_DEFAULT", "W (DNS-o^)");
define("PQL_LDAP_MAILHOST_USERDEFINED", "[U[Íè`³êÜµ½");
define("PQL_LDAP_MAILHOST_UNCHANGED", "[T[o[ÍÏX³êÜ¹ñÅµ½");
define("PQL_LDAP_MAILHOST_NOOPTION", "IvVÍÝè³êÄ¢Ü¹ñ");
define("PQL_LDAP_MAILHOST_HELP", "DNSIvVÅÍ±ÌhCÌÅÌMXo^ªÌp³êÜ·");

// mailmessagestore texts
define("PQL_LDAP_MAILMESSAGESTORE_TITLE", "[{bNXÌpX");
define("PQL_LDAP_MAILMESSAGESTORE_HELP", "âÎpXðgíÈ¢ê qmail/control/ldapmessagestore ©çlª ç©¶ßÏX³êÜ·B '..' Â³êÄ¢Ü¹ñ");
define("PQL_LDAP_MAILMESSAGESTORE_ABSOLUTE_NOTALLOWED", "âÎpXÍ configuration ÉæÁÄmè³êÜ·");
define("PQL_LDAP_MAILMESSAGESTORE_NONE", " èÜ¹ñ");
define("PQL_LDAP_MAILMESSAGESTORE_CHANGE", "[{bNXÌpXÌÏX");
define("PQL_LDAP_MAILMESSAGESTORE_CHANGE_OK", "[{bNXÌpXÍ %path% ÉÏX³êÜµ½");
define("PQL_LDAP_MAILMESSAGESTORE_CHANGE_FAILED", "[{bNXÌpXÌÏXÉ¸sµÜµ½");
define("PQL_LDAP_MAILMESSAGESTORE_UNCHANGED", "[{bNXÌpXÍÏX³êÜ¹ñÅµ½");

// qmaildotmode texts
define("PQL_LDAP_QMAILDOTMODE_TITLE", "dotmode"); // to translate
define("PQL_LDAP_QMAILDOTMODE_HELP", "If you set dotmode to both or ldapwithprog you should add a deliveryprogram too");
define("PQL_LDAP_QMAILDOTMODE_NONE", "default"); // to translate
define("PQL_LDAP_QMAILDOTMODE_CHANGE", "Change dotmode"); // to translate
define("PQL_LDAP_QMAILDOTMODE_CHANGE_OK", "Successfully changed dotmode to %mode%"); // to translate
define("PQL_LDAP_QMAILDOTMODE_CHANGE_FAILED", "Failed changing dotmode"); // to translate
define("PQL_LDAP_QMAILDOTMODE_UNCHANGED", "dotmode unchanged"); // to translate
define("PQL_LDAP_QMAILDOTMODE_MODE_DEFAULT", "default (taken from ldapdefaultdotmode)"); // to translate
define("PQL_LDAP_QMAILDOTMODE_MODE_BOTH", "both (deliveryProgramPath + .qmail)"); // to translate
define("PQL_LDAP_QMAILDOTMODE_MODE_LDAPONLY", "ldaponly (deliveryProgramPath + .qmail ignored for ldap deliveries)"); // to translate
define("PQL_LDAP_QMAILDOTMODE_MODE_LDAPWITHPROG", "ldapwithprog (deliveryProgramPath used, .qmail ignored)"); // to translate
define("PQL_LDAP_QMAILDOTMODE_MODE_NONE", "none (deliveryProgramPath + .qmail ignored)"); // to translate

// deliveryprogrampath texts
define("PQL_LDAP_DELIVERYPROGRAMPATH_TITLE", "Deliveryprogram"); // to translate
define("PQL_LDAP_DELIVERYPROGRAMPATH_HELP", "This is the same as '|/path/to/someprog' in a .qmail file. Only used if qmailDotMode is set to ldapwithprog or both."); // to translate
define("PQL_LDAP_DELIVERYPROGRAMPATH_NONE", "none"); // to translate
define("PQL_LDAP_DELIVERYPROGRAMPATH_CHANGE", "Change path to deliveryprogram"); // to translate
define("PQL_LDAP_DELIVERYPROGRAMPATH_CHANGE_OK", "Successfully changed path to %path%"); // to translate
define("PQL_LDAP_DELIVERYPROGRAMPATH_CHANGE_FAILED", "Failed changing path"); // to translate
define("PQL_LDAP_DELIVERYPROGRAMPATH_UNCHANGED", "deliveryprogram path unchanged"); // to translate
define("PQL_LDAP_DELIVERYPROGRAMPATH_DOTWARNING", "The qmaildotmode of this user is set to %qmaildotmode%. Please to set it to both or ldapwithprog to get the deliveryprogram running."); // to translate

// search engine texts
define("PQL_SEARCH_TITLE","Find Users Whose"); // to translate
define("PQL_SEARCH_TITLE2","Search Results"); // to translate
define("PQL_SEARCH_UID","Username"); // to translate
define("PQL_SEARCH_CN","Common Name"); // to translate
define("PQL_SEARCH_MAIL","Mail Address"); // to translate
define("PQL_SEARCH_MAILALTERNATEADDRESS","Mail Alias"); // to translate
define("PQL_SEARCH_MAILFORWARDINGADDRESS","Forwarding Address"); // to translate
define("PQL_SEARCH_CONTAINS","Contains"); // to translate
define("PQL_SEARCH_IS","Is"); // to translate
define("PQL_SEARCH_STARTSWITH","Starts With"); // to translate
define("PQL_SEARCH_ENDSWITH","Ends With"); // to translate
define("PQL_SEARCH_FINDBUTTON","find"); // to translate

// ldap connection test texts
define("PQL_TEST_TITLE", "phpQLAdmin ldap connection"); // to translate
define("PQL_TEST_TESTS", "Tests"); // to translate
define("PQL_TEST_HELP", "This is a simple test to show if the ldap extension is loaded and the connections are working. It does not provide tests on access rights in the directory."); // to translate
define("PQL_TEST_LDAP_EXT_TITLE", "php ldap extension"); // to translate
define("PQL_TEST_LDAP_EXT_NA", "not available"); // to translate
define("PQL_TEST_LDAP_EXT_OK", "built in (loaded)"); // to translate
define("PQL_TEST_CONNECTION_FAILED", "failed"); // to translate
define("PQL_TEST_CONNECTION_RESOLVE_ERR", "the hostname %host% could not be resolved"); // to translate
define("PQL_TEST_CONNECTION_CONNECT_ERR", "could not connect to port 389 at %host%, please make sure the service is up and it is not blocked with a firewall"); // to translate
define("PQL_TEST_CONNECTION_CONNECT_OK", "ok, have fun ;-)"); // to translate
define("PQL_TEST_CONNECTION_USER_TITLE", "user directory connection"); // to translate
define("PQL_TEST_CONNECTION_USER_BIND_ERR", "connection ok, but could not bind to the directory. please check PQL_LDAP_ROOTDN and PQL_LDAP_ROOTPW"); // to translate
define("PQL_TEST_CONNECTION_CONROL_TITLE", "control directory connection"); // to translate
define("PQL_TEST_CONNECTION_CONTROL_BIND_ERR", "connection ok, but could not bind to the directory. please check PQL_LDAP_CONTROL_ROOTDN and PQL_LDAP_CONTROL_ROOTPW"); // to translate
define("PQL_TEST_CONNECTION_CONROL_INACTIVE", "control extension deactivated"); // to translate
?>