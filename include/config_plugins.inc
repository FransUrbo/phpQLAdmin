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
// $Id: config_plugins.inc,v 2.5 2003-11-19 16:37:41 turbo Exp $

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_ISACTIVE"),
					"attrib.accountstatus.inc",
					"",
					pql_get_define("PQL_GLOB_ATTR_ISACTIVE"));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_EXPIRATION"),
					"attrib.accountstatus.inc",
					"",
					pql_get_define("PQL_GLOB_ATTR_EXPIRATION"));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_MODE"),
					"attrib.deliverymode.inc",
					"",
					pql_get_define("PQL_GLOB_ATTR_MODE"));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_QUOTA"),
					"attrib.mailquota.inc",
					"",
					pql_get_define("PQL_GLOB_ATTR_QUOTA"));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_MAILHOST"),
					"attrib.mailhost.inc",
					"",
					pql_get_define("PQL_GLOB_ATTR_MAILHOST"));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_DEFAULTDOMAIN"),
					"attrib.defaultdomain.inc",
					"",
					pql_get_define("PQL_GLOB_ATTR_DEFAULTDOMAIN"));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_BASEHOMEDIR"),
					"attrib.basehomedir.inc",
					"",
					pql_get_define("PQL_GLOB_ATTR_BASEHOMEDIR"));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_BASEMAILDIR"),
					"attrib.basemaildir.inc",
					"",
					pql_get_define("PQL_GLOB_ATTR_BASEMAILDIR"));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_BASEQUOTA"),
					"attrib.mailquota.inc",
					"",
					pql_get_define("PQL_GLOB_ATTR_BASEQUOTA"));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_ADMINISTRATOR"),
					"attrib.administrator.inc",
					"",
					array(pql_get_define("PQL_GLOB_ATTR_ADMINISTRATOR"),
						  pql_get_define("PQL_GLOB_ATTR_EZMLMADMINISTRATOR"),
						  pql_get_define("PQL_GLOB_ATTR_SEEALSO")));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_MAXIMUMDOMAINUSERS"),
					"attrib.maximumdomainusers.inc",
					"",
					pql_get_define("PQL_GLOB_ATTR_MAXIMUMDOMAINUSERS"));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_MAXIMUMMAILINGLISTS"),
					"attrib.maximummailinglists.inc",
					"",
					pql_get_define("PQL_GLOB_ATTR_MAXIMUMMAILINGLISTS"));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_O"),
					"attrib.outlook.inc",
					"",
					array(pql_get_define("PQL_GLOB_ATTR_O"),
						  pql_get_define("PQL_GLOB_ATTR_POSTALCODE"),
						  pql_get_define("PQL_GLOB_ATTR_POSTALADDRESS"),
						  pql_get_define("PQL_GLOB_ATTR_STREET"),
						  pql_get_define("PQL_GLOB_ATTR_L"),
						  pql_get_define("PQL_GLOB_ATTR_ST"),
						  pql_get_define("PQL_GLOB_ATTR_C"),
						  pql_get_define("PQL_GLOB_ATTR_TELEPHONENUMBER"),
						  pql_get_define("PQL_GLOB_ATTR_FACSIMILETELEPHONENUMBER"),
						  pql_get_define("PQL_GLOB_ATTR_POSTOFFICEBOX"),
						  pql_get_define("PQL_GLOB_ATTR_HOMEPHONE"),
						  pql_get_define("PQL_GLOB_ATTR_MOBILE"),
						  pql_get_define("PQL_GLOB_ATTR_HOMEPOSTALADDRESS"),
						  pql_get_define("PQL_GLOB_ATTR_TITLE"),
						  pql_get_define("PQL_GLOB_ATTR_PHYSICALDELIVERYOFFICENAME"),
						  pql_get_define("PQL_GLOB_ATTR_PAGER"),
						  pql_get_define("PQL_GLOB_ATTR_INFO"),
						  pql_get_define("PQL_GLOB_ATTR_PERSONALTITLE"),
						  pql_get_define("PQL_GLOB_ATTR_ROOMNUMBER")));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_ADDITIONALDOMAINNAME"),
					"attrib.additionaldomainname.inc",
					"",
					pql_get_define("PQL_GLOB_ATTR_ADDITIONALDOMAINNAME"));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_DEFAULTPASSWORDSCHEME"),
					"attrib.defaultpasswordscheme.inc",
					"",
					pql_get_define("PQL_GLOB_ATTR_DEFAULTPASSWORDSCHEME"));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_AUTOCREATEUSERNAME"),
					"attrib.domaintoggle.inc",
					"",
					array(pql_get_define("PQL_GLOB_ATTR_AUTOCREATEUSERNAME"),
						  pql_get_define("PQL_GLOB_ATTR_AUTOCREATEMAILADDRESS")));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_USERNAMEPREFIX"),
					"attrib.usernameprefix.inc",
					"",
					pql_get_define("PQL_GLOB_ATTR_USERNAMEPREFIX"));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_MAILALTERNATE"),
					"attrib.mailalternateaddress.inc",
					"",
					pql_get_define("PQL_GLOB_ATTR_MAILALTERNATE"));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_FORWARDS"),
					"attrib.mailforwardingaddress.inc",
					"",
					pql_get_define("PQL_GLOB_ATTR_FORWARDS"));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_PASSWD"),
					"attrib.userpassword.inc",
					"",
					pql_get_define("PQL_GLOB_ATTR_PASSWD"));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_MAIL"),
					"attrib.mail.inc",
					"",
					pql_get_define("PQL_GLOB_ATTR_MAIL"));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_UID"),
					"attrib.uid.inc",
					"",
					pql_get_define("PQL_GLOB_ATTR_UID"));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_MAILSTORE"),
					"attrib.mailmessagestore.inc",
					"",
					pql_get_define("PQL_GLOB_ATTR_MAILSTORE"));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_HOMEDIR"),
					"attrib.homedirectory.inc",
					"",
					pql_get_define("PQL_GLOB_ATTR_HOMEDIR"));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_DOTMODE"),
					"attrib.qmaildotmode.inc",
					"",
					pql_get_define("PQL_GLOB_ATTR_DOTMODE"));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_PROGRAM"),
					"attrib.deliveryprogrampath.inc",
					"",
					pql_get_define("PQL_GLOB_ATTR_PROGRAM"));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_LOGINSHELL"),
					"attrib.loginshell.inc",
					"",
					pql_get_define("PQL_GLOB_ATTR_LOGINSHELL"));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_CN"),
					"attrib.cn.inc",
					"",
					pql_get_define("PQL_GLOB_ATTR_CN"));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_OPENLDAPACI"),
					"attrib.aci.inc",
					"",
					pql_get_define("PQL_GLOB_ATTR_OPENLDAPACI"));

pql_plugin_register("modrdn",
					"attrib.modrdn.inc",
					"",
					"modrdn");

// List all registred attributes and their files
// - Uncomment this piece of code to view which
//   plugins are registred and to which file
//   they belong.

/*
$i = 0;

foreach($_pql_plugins->attribs as $key => $value){
	echo "$key --> attrib.control.$value.inc <br>";
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
