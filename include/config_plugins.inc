<?php
// ----------------------------
// config_plugins.inc
// phpQLAdmin Plugin Configuration
//
// You're not really supposed to change this file, unless your know
// exactly what you're doing! The intention with this file is to
// avoid hardcoding the attribute names in the code, but instead
// set variables that is used in the code instead.
//
// If you want to customise phpQLAdmin, do NOT do it here, the file
// to customise is 'config_attributes.inc'.
//
// $Id: config_plugins.inc,v 2.12 2004-03-13 09:12:25 turbo Exp $

pql_plugin_register(pql_get_define("PQL_ATTR_ISACTIVE"),
					"attrib.accountstatus.inc",
					"",
					pql_get_define("PQL_ATTR_ISACTIVE"));

pql_plugin_register(pql_get_define("PQL_ATTR_EXPIRATION"),
					"attrib.accountstatus.inc",
					"",
					pql_get_define("PQL_ATTR_EXPIRATION"));

pql_plugin_register(pql_get_define("PQL_ATTR_MODE"),
					"attrib.deliverymode.inc",
					"",
					pql_get_define("PQL_ATTR_MODE"));

pql_plugin_register(pql_get_define("PQL_ATTR_QUOTA_VALUE"),
					"attrib.mailquota.inc",
					"",
					pql_get_define("PQL_ATTR_QUOTA_VALUE"));

pql_plugin_register(pql_get_define("PQL_ATTR_QUOTA_SIZE"),
					"attrib.mailquota.inc",
					"",
					pql_get_define("PQL_ATTR_QUOTA_SIZE"));

pql_plugin_register(pql_get_define("PQL_ATTR_QUOTA_COUNT"),
					"attrib.mailquota.inc",
					"",
					pql_get_define("PQL_ATTR_QUOTA_COUNT"));

pql_plugin_register(pql_get_define("PQL_ATTR_MAILHOST"),
					"attrib.mailhost.inc",
					"",
					pql_get_define("PQL_ATTR_MAILHOST"));

pql_plugin_register(pql_get_define("PQL_ATTR_DEFAULTDOMAIN"),
					"attrib.defaultdomain.inc",
					"",
					pql_get_define("PQL_ATTR_DEFAULTDOMAIN"));

pql_plugin_register(pql_get_define("PQL_ATTR_BASEHOMEDIR"),
					"attrib.basehomedir.inc",
					"",
					pql_get_define("PQL_ATTR_BASEHOMEDIR"));

pql_plugin_register(pql_get_define("PQL_ATTR_BASEMAILDIR"),
					"attrib.basemaildir.inc",
					"",
					pql_get_define("PQL_ATTR_BASEMAILDIR"));

pql_plugin_register(pql_get_define("PQL_ATTR_BASEQUOTA"),
					"attrib.mailquota.inc",
					"",
					pql_get_define("PQL_ATTR_BASEQUOTA"));

pql_plugin_register(pql_get_define("PQL_ATTR_ADMINISTRATOR"),
					"attrib.administrator.inc",
					"",
					array(pql_get_define("PQL_ATTR_ADMINISTRATOR"),
						  pql_get_define("PQL_ATTR_ADMINISTRATOR_EZMLM"),
						  pql_get_define("PQL_ATTR_SEEALSO")));

pql_plugin_register(pql_get_define("PQL_ATTR_MAXIMUM_DOMAIN_USERS"),
					"attrib.maximumdomainusers.inc",
					"",
					pql_get_define("PQL_ATTR_MAXIMUM_DOMAIN_USERS"));

pql_plugin_register(pql_get_define("PQL_ATTR_MAXIMUM_MAILING_LISTS"),
					"attrib.maximummailinglists.inc",
					"",
					pql_get_define("PQL_ATTR_MAXIMUM_MAILING_LISTS"));

pql_plugin_register(pql_get_define("PQL_ATTR_O"),
					"attrib.outlook.inc",
					"",
					array(pql_get_define("PQL_ATTR_O"),
						  pql_get_define("PQL_ATTR_POSTALCODE"),
						  pql_get_define("PQL_ATTR_POSTALADDRESS"),
						  pql_get_define("PQL_ATTR_STREET"),
						  pql_get_define("PQL_ATTR_STREETADDRESS"),
						  pql_get_define("PQL_ATTR_REGISTEREDADDRESS"),
						  pql_get_define("PQL_ATTR_VAT_NUMBER"),
						  pql_get_define("PQL_ATTR_L"),
						  pql_get_define("PQL_ATTR_ST"),
						  pql_get_define("PQL_ATTR_C"),
						  pql_get_define("PQL_ATTR_TELEPHONENUMBER"),
						  pql_get_define("PQL_ATTR_FACSIMILETELEPHONENUMBER"),
						  pql_get_define("PQL_ATTR_POSTOFFICEBOX"),
						  pql_get_define("PQL_ATTR_HOMEPHONE"),
						  pql_get_define("PQL_ATTR_MOBILE"),
						  pql_get_define("PQL_ATTR_HOMEPOSTALADDRESS"),
						  pql_get_define("PQL_ATTR_TITLE"),
						  pql_get_define("PQL_ATTR_PHYSICALDELIVERYOFFICENAME"),
						  pql_get_define("PQL_ATTR_PAGER"),
						  pql_get_define("PQL_ATTR_INFO"),
						  pql_get_define("PQL_ATTR_PERSONALTITLE"),
						  pql_get_define("PQL_ATTR_ROOMNUMBER")));

pql_plugin_register(pql_get_define("PQL_ATTR_ADDITIONAL_DOMAINNAME"),
					"attrib.additionaldomainname.inc",
					"",
					pql_get_define("PQL_ATTR_ADDITIONAL_DOMAINNAME"));

pql_plugin_register(pql_get_define("PQL_ATTR_DEFAULT_PASSWORDSCHEME"),
					"attrib.defaultpasswordscheme.inc",
					"",
					pql_get_define("PQL_ATTR_DEFAULT_PASSWORDSCHEME"));

pql_plugin_register(pql_get_define("PQL_ATTR_AUTOCREATE_USERNAME"),
					"attrib.domaintoggle.inc",
					"",
					array(pql_get_define("PQL_ATTR_AUTOCREATE_USERNAME"),
						  pql_get_define("PQL_ATTR_AUTOCREATE_MAILADDRESS")));

pql_plugin_register(pql_get_define("PQL_ATTR_USERNAME_PREFIX"),
					"attrib.usernameprefix.inc",
					"",
					array(pql_get_define("PQL_ATTR_USERNAME_PREFIX"),
						  pql_get_define("PQL_ATTR_USERNAME_PREFIX_LENGTH")));

pql_plugin_register(pql_get_define("PQL_ATTR_MAILALTERNATE"),
					"attrib.mailalternateaddress.inc",
					"",
					pql_get_define("PQL_ATTR_MAILALTERNATE"));

pql_plugin_register(pql_get_define("PQL_ATTR_FORWARDS"),
					"attrib.mailforwardingaddress.inc",
					"",
					pql_get_define("PQL_ATTR_FORWARDS"));

pql_plugin_register(pql_get_define("PQL_ATTR_PASSWD"),
					"attrib.userpassword.inc",
					"",
					pql_get_define("PQL_ATTR_PASSWD"));

pql_plugin_register(pql_get_define("PQL_ATTR_MAIL"),
					"attrib.mail.inc",
					"",
					pql_get_define("PQL_ATTR_MAIL"));

pql_plugin_register(pql_get_define("PQL_ATTR_UID"),
					"attrib.uid.inc",
					"",
					pql_get_define("PQL_ATTR_UID"));

pql_plugin_register(pql_get_define("PQL_ATTR_QMAILUID"),
					"attrib.ugidnumber.inc",
					"",
					array(pql_get_define("PQL_ATTR_QMAILUID"),
						  pql_get_define("PQL_ATTR_QMAILGID")));

pql_plugin_register(pql_get_define("PQL_ATTR_MAILSTORE"),
					"attrib.mailmessagestore.inc",
					"",
					pql_get_define("PQL_ATTR_MAILSTORE"));

pql_plugin_register(pql_get_define("PQL_ATTR_HOMEDIR"),
					"attrib.homedirectory.inc",
					"",
					pql_get_define("PQL_ATTR_HOMEDIR"));

pql_plugin_register(pql_get_define("PQL_ATTR_DOTMODE"),
					"attrib.qmaildotmode.inc",
					"",
					pql_get_define("PQL_ATTR_DOTMODE"));

pql_plugin_register(pql_get_define("PQL_ATTR_PROGRAM"),
					"attrib.deliveryprogrampath.inc",
					"",
					pql_get_define("PQL_ATTR_PROGRAM"));

pql_plugin_register(pql_get_define("PQL_ATTR_LOGINSHELL"),
					"attrib.loginshell.inc",
					"",
					pql_get_define("PQL_ATTR_LOGINSHELL"));

pql_plugin_register(pql_get_define("PQL_ATTR_CN"),
					"attrib.cn.inc",
					"",
					pql_get_define("PQL_ATTR_CN"));

pql_plugin_register(pql_get_define("PQL_ATTR_LDAPACI"),
					"attrib.aci.inc",
					"",
					pql_get_define("PQL_ATTR_LDAPACI"));

pql_plugin_register("modrdn",
					"attrib.modrdn.inc",
					"",
					"modrdn");

pql_plugin_register(pql_get_define("PQL_ATTR_ADDITIONAL_GROUP"),
					"attrib.additionalgroup.inc",
					"",
					pql_get_define("PQL_ATTR_ADDITIONAL_GROUP"));

// List all registred attributes and their files
// - Uncomment this piece of code to view which
//   plugins are registred and to which file
//   they belong.

/*
$i = 0;
foreach($_pql_plugins->attribs as $key => $value){
	echo "$key --> attrib.$value.inc <br>";
	$i++;
}
echo "Attribute count: $i<br><br>";
*/

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>