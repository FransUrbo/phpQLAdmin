<?php
// lang.en.inc
// english language definition file
// $Id: lang.en.inc,v 2.11 2003-01-24 09:06:57 turbo Exp $
//
// if you have a translation to another language, please contact
// me --> Matthias Blaser <mb@adfinis.ch>

// Should we refere to the LDAP branch as 'branch' or 'domain'?
if(PQL_LDAP_REFERENCE_DOMAINS_WITH == "dc") {
	$DOMAIN = "domain";
} else {
	$DOMAIN = "branch";
}

// language definition
define("PQL_LANG","en");

// welcome texts (home.php)
define("PQL_WELCOME", "Welcome to ");
define("PQL_DESCRIPTION","The QMail-LDAP Adminstration");

// login texts
define("PQL_LOGIN","Please Login");
define("PQL_USERNAME","Login ID");
define("PQL_USERPASS","Password");
define("PQL_RETYPE","Retype password");
define("PQL_ORG_BASE","Organization");

define("PQL_HOME","Home");
define("PQL_LOGOUT","Log out");

// general texts
define("PQL_SAVE", "Save");
define("PQL_OPTIONS", "Options");
define("PQL_EMAIL", "Email");
define("PQL_ACTIONS", "Actions");

define("PQL_SURE", "Are you really sure ");
define("PQL_YES", "Yes");
define("PQL_NO", "No");
define("PQL_ADD", "create");

define("PQL_DOCUMENTATION", "Documentation");
define("PQL_SHOW_CONFIG", "Show phpQLAdmin configuration");
define("PQL_TEST_LDAP", "Test LDAP-Connection");

define("PQL_UNSET", "<i>Not set</i>");

// error messages
define("PQL_INVALID", "invalid");
define("PQL_MISSING", "missing");
define("PQL_EXISTS", "already exists");
define("PQL_FAILED_RETYPE", "not exactly retyped");
define("PQL_NO_OBJECT", "attribute not found");
define("PQL_GREATER_ZERO", "must be > 0");
define("PQL_SESSION_REG", "Could not register session variables");
define("PQL_ERROR", "Error");

// user/domain/address things
define("PQL_USER", "User");
define("PQL_USER_ID", "Username");
define("PQL_USER_DATA", "User data");
define("PQL_USER_DATA_SURNAME", "Surname");
define("PQL_USER_DATA_LASTNAME", "Lastname");
define("PQL_USER_DATA_NAME", "Name");
define("PQL_USER_HOMEDIR", "Home directory");
define("PQL_USER_MAILDIR", "Mail directory");
define("PQL_USER_LOGINSHELL", "Login shell");

define("PQL_USER_REGISTRED","Registred user");
define("PQL_USER_NEW", "Register new user");
define("PQL_USER_DELETE", "Delete user");
define("PQL_USER_DELETE_FORWARDS", "Also delete forwards to this user");
define("PQL_USER_EDIT", "Change user data");
define("PQL_USER_EXIST", "User %user% already exists");
define("PQL_USER_NONE", "No users registred");
define("PQL_USER_MAILBOXPROPERTIES", "Mailbox properties");
define("PQL_USER_ADD", "Create user");
define("PQL_USER_ADD_TITLE", "Create user in $DOMAIN %domain%");
define("PQL_USER_ADD_HELP1", "The combination of surname and name (e.g. Zaphod Beeblebrox) must be unique in whole $DOMAIN. The username must be unique in whole system.");
define("PQL_USER_ADD_HELP2", "The email address and the username will be automatically converted to lowercase");
define("PQL_USER_ADD_OK", "Successfully created the new user");
define("PQL_USER_ADD_FAILED", "Failed to create the new user");
define("PQL_USER_ADD_SCRIPT_OK", "Successfully executed the user add script");
define("PQL_USER_ADD_SCRIPT_FAILED", "The user add script failed!");
define("PQL_USER_ACCOUNT_PROPERTIES", "Account properties");
define("PQL_USER_ACCOUNT_PROPERTIES_MORE", "Additional account properties");
define("PQL_USER_ACCOUNT_TYPE", "Type");
define("PQL_USER_DEL_TITLE", "Remove user '%user%' from $DOMAIN '%domain%'");
define("PQL_USER_DEL_OK", "Successfully removed user");
define("PQL_USER_DEL_FAILED", "Failed removing the user");
define("PQL_USER_DEL_ATTRIBUTE_TITLE", "Remove attribute (%value%)");
define("PQL_USER_DEL_ATTRIBUTE_OK", "Attribute removed");
define("PQL_USER_DEL_ATTRIBUTE_FAILED", "Failed removing attribute");
define("PQL_USER_DELIVERYPROPERTIES","Advanced delivery properties");

define("PQL_DNS_TITLE", "DNS informations");
define("PQL_DNS_MAILHOST", "Mailhost");
define("PQL_DNS_PRIORITY", "Priority");
define("PQL_DNS_NONE", "This domain isn't registred within the DNS system");

define("PQL_DOMAIN_CHANGE_ATTRIBUTE_TITLE", "Change values of all users in this $DOMAIN");
define("PQL_DOMAIN_CHANGE_ATTRIBUTE", "Attribute");
define("PQL_DOMAIN_INVALID", "Invalid domain name !! use: domain.tld (e.g. adfinis.com)");
define("PQL_DOMAIN_EXISTS", "This $DOMAIN already exists");
define("PQL_DOMAIN_ADD", "Create $DOMAIN");
if(PQL_LDAP_REFERENCE_DOMAINS_WITH == "dc") {
	define("PQL_DOMAIN_ADD_OK", "Domain %domain% successfully created.<br>You have to manually add this domain to the rpcthosts and locals files in the qmail/control directory !!");
} else {
	define("PQL_DOMAIN_ADD_OK", "Branch %domain% successfully created.");
}
define("PQL_DOMAIN_ADD_SCRIPT_OK", "Successfully executed the $DOMAIN add script");
define("PQL_DOMAIN_ADD_SCRIPT_FAILED", "The $DOMAIN add script failed!");
define("PQL_DOMAIN_ADD_FAILED", "Failed creating the $DOMAIN");
define("PQL_DOMAIN_ADD_INFO_DC", "Using $DOMAIN mode (dc separated).<br>Don't use '.' (dots) in $DOMAIN name");
define("PQL_DOMAIN_ADD_INFO_OU", "Using organization layout (ou separated).<br>Make sure youre using 'domain.tld' for domain name");
define("PQL_DOMAIN_DEL", "Remove $DOMAIN");
define("PQL_DOMAIN_DEL_FORWARDS", "Also delete forwards to users in this $DOMAIN");
define("PQL_DOMAIN_DEL_TITLE", "Remove $DOMAIN %domain%");
define("PQL_DOMAIN_DEL_WARNING", "<b>Attention:</b> If you deleted a $DOMAIN, all users within this $DOMAIN will be deleted too !!");
define("PQL_DOMAIN_DEL_OK", "Successfully removed the $DOMAIN");
define("PQL_DOMAIN_DEL_FAILED", "Failed removing $DOMAIN");
define("PQL_DOMAIN_DEFAULT_TITLE", "Change default values for this $DOMAIN");
define("PQL_DOMAIN_DEFAULT_VALUES", "Default $DOMAIN values");
define("PQL_DOMAIN_DEFAULT_NAME", "Default domain name");
define("PQL_DOMAIN_DEFAULT_HOMEDIR", "Default homedirectory for users");
define("PQL_DOMAIN_DEFAULT_MAILDIR", "Default maildirectory for users");
define("PQL_DOMAIN_DEFAULT_CHANGE_NAME", "Change default $DOMAIN name for $DOMAIN ");
define("PQL_DOMAIN_DEFAULT_CHANGE_HOMEDIR", "Change default homedirectory base for $DOMAIN ");
define("PQL_DOMAIN_DEFAULT_CHANGE_MAILDIR", "Change default maildirectory base for $DOMAIN ");
define("PQL_DOMAIN_DEFAULT_CHANGE_OK", "Successfully changed default domainname");
define("PQL_DOMAIN_DEFAULT_CHANGE_FAILED", "Failed to change the default domainname");
define("PQL_DOMAIN_DEFAULT_CHANGE_UNCHANGED", "Default domainname unchanged");

// database administrator texts
define("PQL_DOMAIN_ADMIN_TITLE", "Domain administrator(s)");
define("PQL_DOMAIN_ADMIN_DELETE", "Remove domain administrator");
define("PQL_DOMAIN_ADMIN_DELETE_OK", "Administrator removed successfully");
define("PQL_DOMAIN_ADMIN_DELETE_FAILED", "Could not delete administrator");
define("PQL_DOMAIN_ADMIN_ADD", "Add domain administrator for domain ");
define("PQL_DOMAIN_ADMIN_ADD_OK", "Administrator added successfully");
define("PQL_DOMAIN_ADMIN_ADD_FAILED", "Could not add administrator");
define("PQL_DOMAIN_ADMIN_CHANGE", "Administrator DN");
define("PQL_DOMAIN_ADMIN_CHANGE_OK", "Administrator added successfully");
define("PQL_DOMAIN_ADMIN_CHANGE_FAILED", "Could not add administrator DN");
define("PQL_DOMAIN_ADMIN_UNCHANGED", "Domain administrator unchanged");

define("PQL_ADDRESS_REGISTRED", "Registred addresses");
define("PQL_ADDRESS_TYPE", "Type");

define("PQL_SENDMAIL", "Send testmail");
define("PQL_SENDMAIL_OK", "Successfully sended mail to %email%");
define("PQL_SENDMAIL_FAILED", "Failed sending mail");

// deliverymode texts
// (normal, forwardonly, nombox, localdelivery, reply, echo)
define("PQL_LDAP_DELIVERYMODE_TITLE","Deliverymode");
define("PQL_LDAP_DELIVERYMODE_MODE","Mode");
define("PQL_LDAP_DELIVERYMODE_NORMAL","Normal");
define("PQL_LDAP_DELIVERYMODE_FORWARDONLY","Only forward");
define("PQL_LDAP_DELIVERYMODE_NOMBOX","No local mailbox");
define("PQL_LDAP_DELIVERYMODE_LOCALDELIVERY","Save in local mailbox");
define("PQL_LDAP_DELIVERYMODE_REPLY","Do auto reply");
define("PQL_LDAP_DELIVERYMODE_REPLY_SEND","Send an auto reply");
define("PQL_LDAP_DELIVERYMODE_REPLY_TEXT","Text for replying");
define("PQL_LDAP_DELIVERYMODE_ECHO","Echo to console (tricky)");
define("PQL_LDAP_DELIVERYMODE_CHANGE","Change properties");
define("PQL_LDAP_DELIVERYMODE_CHANGE_OK","Deliverymode changed");
define("PQL_LDAP_DELIVERYMODE_CHANGE_FAILED","Failed changing deliverymode");
define("PQL_LDAP_DELIVERYMODE_NULL","Default properties");
define("PQL_LDAP_DELIVERYMODE_PROFILE", "Profile");
define("PQL_LDAP_DELIVERYMODE_PROFILE_LOCAL", "Mail account");
define("PQL_LDAP_DELIVERYMODE_PROFILE_FORWARD", "Forward account");
define("PQL_LDAP_DELIVERYMODE_PROFILE_SYSTEM", "System account");

define("PQL_LDAP_DELIVERYMODE_PROFILE_INC", " includes: ");
define("PQL_LDAP_DELIVERYMODE_PROFILE_LOCAL_INFO", "local mailbox, POP account");
define("PQL_LDAP_DELIVERYMODE_PROFILE_FORWARD_INFO", "forward only, no local mailbox");
define("PQL_LDAP_DELIVERYMODE_PROFILE_SYSTEM_INFO", "loginshell, homedir");

// accountstatus texts
// (active, nopop, disable)
define("PQL_LDAP_ACCOUNTSTATUS_TITLE","Account status");
define("PQL_LDAP_ACCOUNTSTATUS_STATUS","Status");
define("PQL_LDAP_ACCOUNTSTATUS_ACTIVE","active");
define("PQL_LDAP_ACCOUNTSTATUS_NOPOP","POP locked");
define("PQL_LDAP_ACCOUNTSTATUS_DISABLE","locked");
define("PQL_LDAP_ACCOUNTSTATUS_CHANGE","Set status");
define("PQL_LDAP_ACCOUNTSTATUS_CHANGE_OK","Successfully set status to %status%");
define("PQL_LDAP_ACCOUNTSTATUS_CHANGE_FAILED","Failed setting status");
define("PQL_LDAP_ACCOUNTSTATUS_CHANGE_ACTIVE","activate");
define("PQL_LDAP_ACCOUNTSTATUS_CHANGE_NOPOP","lock POP account");
define("PQL_LDAP_ACCOUNTSTATUS_CHANGE_DISABLE","lock");

// mail texts
define("PQL_LDAP_MAIL_TITLE","Main address");
define("PQL_LDAP_MAIL_CHANGE","Change main address");
define("PQL_LDAP_MAIL_HELP", "An address must be unique in whole domain");
define("PQL_LDAP_MAIL_CHANGE_OK", "Successfully changed main address from %old% to %new%");
define("PQL_LDAP_MAIL_CHANGE_FAILED", "Failed changing main address");
define("PQL_LDAP_MAIL_UNCHANGED", "Main address unchanged");

// mailforwardingaddress texts
define("PQL_LDAP_MAILFORWARDINGADDRESS_TITLE","Forward");
define("PQL_LDAP_MAILFORWARDINGADDRESS_OTHER", "Forwarders of other accounts to this user");
define("PQL_LDAP_MAILFORWARDINGADDRESS_NEW","Create new forwarding address");
define("PQL_LDAP_MAILFORWARDINGADDRESS_ADD_HELP", "You can add more forwarding address on user's details page");
define("PQL_LDAP_MAILFORWARDINGADDRESS_ADD_OK", "Successfully created forward to %mail%");
define("PQL_LDAP_MAILFORWARDINGADDRESS_ADD_FAILED", "Failed creating forward");
define("PQL_LDAP_MAILFORWARDINGADDRESS_NONE","none");
define("PQL_LDAP_MAILFORWARDINGADDRESS_CHANGE", "Change forwarding address");
define("PQL_LDAP_MAILFORWARDINGADDRESS_CHANGE_OK", "Successfully changed forwarding address from %old% to %new%");
define("PQL_LDAP_MAILFORWARDINGADDRESS_CHANGE_FAILED", "Failed modifying forwarding address");
define("PQL_LDAP_MAILFORWARDINGADDRESS_UNCHANGED", "Forwarding address unchanged");

// mailalternateaddress texts
define("PQL_LDAP_MAILALTERNATEADDRESS_TITLE","Alias");
define("PQL_LDAP_MAILALTERNATEADDRESS_NEW","Create new alias");
define("PQL_LDAP_MAILALTERNATEADDRESS_DEL", "Delete alias");
define("PQL_LDAP_MAILALTERNATEADDRESS_DEL_FORWARDS", "Also delete forwards to this alias");
define("PQL_LDAP_MAILALTERNATEADDRESS_HELP", "An alias must be unique in whole domain");
define("PQL_LDAP_MAILALTERNATEADDRESS_ADD_OK", "Successfully created alias %mail%");
define("PQL_LDAP_MAILALTERNATEADDRESS_ADD_FAILED", "Failed creating alias");
define("PQL_LDAP_MAILALTERNATEADDRESS_CHANGE", "Change alias");
define("PQL_LDAP_MAILALTERNATEADDRESS_CHANGE_OK", "Successfully changed alias from %old% to %new%");
define("PQL_LDAP_MAILALTERNATEADDRESS_CHANGE_FAILED", "Failed modifying alias");
define("PQL_LDAP_MAILALTERNATEADDRESS_UNCHANGED","Alias unchanged");

// userpassword texts
define("PQL_LDAP_USERPASSWORD_TITLE", "Password");
define("PQL_LDAP_USERPASSWORD_NONE", "none");
define("PQL_LDAP_USERPASSWORD_ENCRYPTED", "encrypted");
define("PQL_LDAP_USERPASSWORD_KERBEROS", "(Kerberos V)");
define("PQL_LDAP_USERPASSWORD_NEW", "Set password");
define("PQL_LDAP_USERPASSWORD_RETYPE", "Password (Retype)");
define("PQL_LDAP_USERPASSWORD_HELP", "A password can only contain numbers and letters, special characters are not allowed");
define("PQL_LDAP_USERPASSWORD_HELP_KRB", "If using {KERBEROS} as password scheme, make sure you<br>include the correct REALM (principal@REALM.TLD)<br><b>Unfortunatly you'll have to create the Kerberos principal yourself, this part isn't finished (yet).</b>");
define("PQL_LDAP_USERPASSWORD_CHANGE_OK", "Successfully changed password");
define("PQL_LDAP_USERPASSWORD_CHANGE_FAILED", "Failed changing password");

// common name texts
define("PQL_LDAP_CN_CHANGE", "Change common name");
define("PQL_LDAP_CN_CHANGE_OK", "Successfully changed the common name to %new%");
define("PQL_LDAP_CN_CHANGE_FAILED", "Failed to change the common name");
define("PQL_LDAP_CN_UNCHANGED", "Common name unchanged");

// homedirectory texts
define("PQL_LDAP_HOMEDIRECTORY_CHANGE", "Change home directory");
define("PQL_LDAP_HOMEDIRECTORY_CHANGE_OK", "Successfully changed homedirectory to %new%");
define("PQL_LDAP_HOMEDIRECTORY_CHANGE_FAILED", "Failed to change the homedirectory");
define("PQL_LDAP_HOMEDIRECTORY_UNCHANGED", "Homedirectory unchanged");

// loginshell texts
define("PQL_LDAP_LOGINSHELL_CHANGE", "Change login shell");
define("PQL_LDAP_LOGINSHELL_CHANGE_OK", "Successfully changed login shell to %new%");
define("PQL_LDAP_LOGINSHELL_CHANGE_FAILED", "Failed to change the login shell");
define("PQL_LDAP_LOGINSHELL_UNCHANGED", "Login shell unchanged");

// uid texts
define("PQL_LDAP_UID_TITLE", "Username");
define("PQL_LDAP_UID_NONE", "none");
define("PQL_LDAP_UID_HELP", "The username can only contain numbers, letters and following special characters &quot;@&quot;, &quot;.&quot;, &quot;_&quot;, &quot;-&quot;. Additionally, the username should be unique in whole system.");
define("PQL_LDAP_UID_HELP_SHORT", "numbers, letters, and following special chars: &quot;@&quot;, &quot;.&quot;, &quot;_&quot;, &quot;-&quot;<br>If left out, a username will be created automatically.");
define("PQL_LDAP_UID_CHANGE", "Change username");
define("PQL_LDAP_UID_CHANGE_OK", "Successfully changed username from %old% to %new%");
define("PQL_LDAP_UID_CHANGE_FAILED", "Failed changing the username");
define("PQL_LDAP_UID_UNCHANGED","Username unchanged");

// mailquota texts
define("PQL_LDAP_MAILQUOTA_TITLE", "Default quota");
define("PQL_LDAP_MAILQUOTA_NONE", "Standard");
define("PQL_LDAP_MAILQUOTA_ECHO_MAXSIZE", "max %maxsize% KB");
define("PQL_LDAP_MAILQUOTA_ECHO_MAXSIZE_UNLIMITED", "unlimited size");
define("PQL_LDAP_MAILQUOTA_ECHO_MAXMAILS", "max %maxmails% mails");
define("PQL_LDAP_MAILQUOTA_ECHO_MAXMAILS_UNLIMITED", "unlimited number of mails");
define("PQL_LDAP_MAILQUOTA_HELP", "With 'Standard', qmail/ldap will get quotas from its configuration file");
define("PQL_LDAP_MAILQUOTA_NONESELECTED", "You must select at least one type of quota");
define("PQL_LDAP_MAILQUOTA_MAXSIZE_TITLE", "Max. size of mailbox");
define("PQL_LDAP_MAILQUOTA_MAXMAILS_TITLE", "Max. number of mails");
define("PQL_LDAP_MAILQUOTA_DEFAULT", "Standard");
define("PQL_LDAP_MAILQUOTA_DEFAULT_CHANGE", "Change default max quota for domain ");
define("PQL_LDAP_MAILQUOTA_USERDEFINED", "User defined");
define("PQL_LDAP_MAILQUOTA_CHANGE", "Change quota");
define("PQL_LDAP_MAILQUOTA_CHANGE_OK", "Successfully set the quota to %quota%");
define("PQL_LDAP_MAILQUOTA_CHANGE_FAILED", "Failed changing the quota");
define("PQL_LDAP_MAILQUOTA_UNCHANGED","Quota unchanged");

// mailhost texts
define("PQL_LDAP_MAILHOST_TITLE", "Mailserver");
define("PQL_LDAP_MAILHOST_FQDN", "Host address (FQDN)");
define("PQL_LDAP_MAILHOST_NONE", "none");
define("PQL_LDAP_MAILHOST_CHANGE", "Change host address of mailserver");
define("PQL_LDAP_MAILHOST_CHANGE_OK", "Successfully changed mailserver to %mailhost%");
define("PQL_LDAP_MAILHOST_CHANGE_FAILED", "Failed changing mailserver");
define("PQL_LDAP_MAILHOST_DEFAULT", "Standard (DNS entry)");
define("PQL_LDAP_MAILHOST_USERDEFINED", "User defined");
define("PQL_LDAP_MAILHOST_UNCHANGED", "Mailserver unchanged");
define("PQL_LDAP_MAILHOST_NOOPTION", "No option is set");
define("PQL_LDAP_MAILHOST_HELP", "With option DNS, the first MX record of this domain will be taken");

// mailmessagestore texts
define("PQL_LDAP_MAILMESSAGESTORE_TITLE", "Path to mailbox");
define("PQL_LDAP_MAILMESSAGESTORE_HELP", "If you don't provide an absolute path, it will be prefixed with the value in qmail/control/ldapmessagestore. '..' are not allowed.");
define("PQL_LDAP_MAILMESSAGESTORE_ABSOLUTE_NOTALLOWED", "Absolute paths are denied by configuration");
define("PQL_LDAP_MAILMESSAGESTORE_NONE", "none");
define("PQL_LDAP_MAILMESSAGESTORE_CHANGE", "Change path to mailbox");
define("PQL_LDAP_MAILMESSAGESTORE_CHANGE_OK", "Successfully changed mailbox path to %path%");
define("PQL_LDAP_MAILMESSAGESTORE_CHANGE_FAILED", "Failed changing mailbox path");
define("PQL_LDAP_MAILMESSAGESTORE_UNCHANGED", "Mailbox path unchanged");

// qmaildotmode texts
define("PQL_LDAP_QMAILDOTMODE_TITLE", "dotmode");
define("PQL_LDAP_QMAILDOTMODE_HELP", "If you set dotmode to both or ldapwithprog you should add a deliveryprogram too");
define("PQL_LDAP_QMAILDOTMODE_NONE", "default");
define("PQL_LDAP_QMAILDOTMODE_CHANGE", "Change dotmode");
define("PQL_LDAP_QMAILDOTMODE_CHANGE_OK", "Successfully changed dotmode to %mode%");
define("PQL_LDAP_QMAILDOTMODE_CHANGE_FAILED", "Failed changing dotmode");
define("PQL_LDAP_QMAILDOTMODE_UNCHANGED", "dotmode unchanged");
define("PQL_LDAP_QMAILDOTMODE_MODE_DEFAULT", "default (taken from ldapdefaultdotmode)");
define("PQL_LDAP_QMAILDOTMODE_MODE_BOTH", "both (deliveryProgramPath + .qmail)");
define("PQL_LDAP_QMAILDOTMODE_MODE_DOTONLY", "dotonly (only .qmail)");
define("PQL_LDAP_QMAILDOTMODE_MODE_LDAPONLY", "ldaponly (deliveryProgramPath + .qmail ignored for ldap deliveries)");
define("PQL_LDAP_QMAILDOTMODE_MODE_LDAPWITHPROG", "ldapwithprog (deliveryProgramPath used, .qmail ignored)");
define("PQL_LDAP_QMAILDOTMODE_MODE_NONE", "none (deliveryProgramPath + .qmail ignored)");

// deliveryprogrampath texts
define("PQL_LDAP_DELIVERYPROGRAMPATH_TITLE", "Deliveryprogram");
define("PQL_LDAP_DELIVERYPROGRAMPATH_HELP", "This is the same as '|/path/to/someprog' in a .qmail file. Only used if qmailDotMode is set to ldapwithprog or both.");
define("PQL_LDAP_DELIVERYPROGRAMPATH_NONE", "none");
define("PQL_LDAP_DELIVERYPROGRAMPATH_CHANGE", "Change path to deliveryprogram");
define("PQL_LDAP_DELIVERYPROGRAMPATH_CHANGE_OK", "Successfully changed path to %path%");
define("PQL_LDAP_DELIVERYPROGRAMPATH_CHANGE_FAILED", "Failed changing path");
define("PQL_LDAP_DELIVERYPROGRAMPATH_UNCHANGED", "deliveryprogram path unchanged");
define("PQL_LDAP_DELIVERYPROGRAMPATH_DOTWARNING", "The qmaildotmode of this user is set to %qmaildotmode%. Please to set it to both or ldapwithprog to get the deliveryprogram running.");

// search engine texts
define("PQL_SEARCH_TITLE","Find Users Whose");
define("PQL_SEARCH_TITLE2","Search Results");
define("PQL_SEARCH_UID","Username");
define("PQL_SEARCH_CN","Common Name");
define("PQL_SEARCH_MAIL","Mail Address");
define("PQL_SEARCH_MAILALTERNATEADDRESS","Mail Alias");
define("PQL_SEARCH_MAILFORWARDINGADDRESS","Forwarding Address");
define("PQL_SEARCH_CONTAINS","Contains");
define("PQL_SEARCH_IS","Is");
define("PQL_SEARCH_STARTSWITH","Starts With");
define("PQL_SEARCH_ENDSWITH","Ends With");
define("PQL_SEARCH_FINDBUTTON","find");

// ldap connection test texts
define("PQL_TEST_TITLE", "phpQLAdmin ldap connection");
define("PQL_TEST_TESTS", "Tests");
define("PQL_TEST_HELP", "This is a simple test to show if the ldap extension is loaded<br>and the connections are working. It does not provide tests<br>on access rights in the directory.");
define("PQL_TEST_LDAP_EXT_TITLE", "php ldap extension");
define("PQL_TEST_LDAP_EXT_NA", "not available");
define("PQL_TEST_LDAP_EXT_OK", "built in (loaded)");
define("PQL_TEST_CONNECTION_FAILED", "failed");
define("PQL_TEST_CONNECTION_RESOLVE_ERR", "the hostname %host% could not be resolved");
define("PQL_TEST_CONNECTION_CONNECT_ERR", "could not connect to port 389 at %host%, please make sure the service is up and it is not blocked with a firewall");
define("PQL_TEST_CONNECTION_CONNECT_OK", "ok, have fun ;-)");
define("PQL_TEST_CONNECTION_USER_TITLE", "user directory connection");
define("PQL_TEST_CONNECTION_USER_BIND_ERR", "connection ok, but could not bind to the directory.");
define("PQL_TEST_CONNECTION_CONROL_TITLE", "control directory connection");
define("PQL_TEST_CONNECTION_CONTROL_BIND_ERR", "connection ok, but could not bind to the directory. please check PQL_LDAP_CONTROL_ROOTDN and PQL_LDAP_CONTROL_ROOTPW");
define("PQL_TEST_CONNECTION_CONROL_INACTIVE", "control extension deactivated");

/*
 * Local variables:
 * mode: php
 * tab-width: 4
 * End:
 */
?>
