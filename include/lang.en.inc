<?php
// lang.en.inc

// This is the alphabet of the language. Uppercased characters only at
// this time. PHP should be able to figure out the lowercased eqvivalent.
$alphabet = array("A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K",
                  "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V",
                  "W", "X", "Y", "Z");

// If you want HTML tags, you can have that in the translation string
// (ie, the value AFTER the '=>' character).
$language = array("%what% unchanged" => "",
				  "A limit of 0 (zero) means unlimited accounts" => "",
				  "A negative value means that no mailinglists are allowed" => "",
				  "A password can only contain numbers and letters, special characters are not allowed" => "",
				  "Absolute paths are denied by configuration" => "",
				  "Access Control Information" => "",
				  "Access control values" => "",
				  "Access to create branch with ACIs in DN %dn%" => "",
				  "Access to create branches in DN %dn%" => "",
				  "Access to modify DN \b%dn%\B" => "",
				  "Access to write phpQLAdmin configuration in DN %dn%" => "",
				  "Account properties" => "",
				  "Account status" => "",
				  "Action" => "",
				  "Actions" => "",
				  "Activate" => "",
				  "Active" => "",
				  "Active Threads" => "",
				  "Add %user% as administrator for domain" => "",
				  "Add %what%" => "",
				  "Add %what% for domain %domain%" => "",
				  "Add %what% to %attribute%" => "",
				  "Add SMTP route" => "",
				  "Add a record to domain %domain%" => "",
				  "Add email address to subscription list" => "",
				  "Add to attribute %attribute%" => "",
				  "Additional" => "",
				  "Additional account properties" => "",
				  "Additional bounce text" => "",
				  "Additional custom text in bounce messages, eg. for providing contact information" => "",
				  "Additional domain name" => "",
				  "Additional group(s)" => "",
				  "Additional option" => "",
				  "Additional quota warning message" => "",
				  "Address" => "",
				  "Administrator" => "",
				  "Administrator DN" => "",
				  "Administrator in branch" => "",
				  "Advanced delivery properties" => "",
				  "Alias" => "",
				  "Allow absolute mailbox paths" => "",
				  "Allow change of LDAP server" => "",
				  "Allow user to administrate Controls information" => "",
				  "Allow user to create domains below %domain%" => "",
				  "Allowed %attrib% for host %host%" => "",
				  "Already exists" => "",
				  "Also delete forwards to this alias" => "",
				  "Also delete forwards to users in this domain" => "",
				  "An alias must be unique to the domain" => "",
				  "An email address must be unique in whole domain" => "",
				  "Archive it" => "",
				  "Are you really sure" => "",
				  "Attention: If you delete a domain, all users, DNS zones etc within this domain will be deleted too" => "",
				  "Attention: If you deleted a domain, all users within this domain will be deleted too" => "",
				  "Attribute" => "",
				  "Attribute deliveryProgramPath is used if existant, .qmail files are ignored" => "",
				  "Automatic reload of navigation bar" => "",
				  "Automatic replication configured" => "",
				  "Automatically add hostname to domain for replication" => "",
				  "Automatically generate %what%" => "",
				  "Automatically replicate domains to %what%" => "",
				  "Average Per Minute" => "",
				  "Base DN" => "",
				  "Base configuration" => "",
				  "Basic LDAP Server Status" => "",
				  "Basic List options" => "",
				  "Blocked senders" => "",
				  "Both (deliveryProgramPath + .qmail)" => "",
				  "Both LDAP attribute and .qmail files are ignored" => "",
				  "Bouncing source" => "",
				  "Bound As" => "",
				  "Branch DN" => "",
				  "Branch base" => "",
				  "Branch configuration" => "",
				  "Branch name" => "",
				  "Branch owner" => "",
				  "Bugtracker" => "",
				  "Built in (loaded)" => "",
				  "Bytes" => "",
				  "CNAME" => "",
				  "Can't find any domains!" => "",
				  "Can't get %what% path from domain '%domain%'!" => "",
				  "Can't get baseMailDir path from %domain%" => "",
				  "Change %what% for domain %domain%" => "",
				  "Change Bind9 value" => "",
				  "Change alias" => "",
				  "Change attribute %attribute%" => "",
				  "Change common name" => "",
				  "Change dotmode" => "",
				  "Change forwarding address" => "",
				  "Change home directory" => "",
				  "Change login shell" => "",
				  "Change main address" => "",
				  "Change options" => "",
				  "Change path to deliveryprogram" => "",
				  "Change path to mailbox" => "",
				  "Change properties" => "",
				  "Change quota" => "",
				  "Change user data" => "",
				  "Change user data for %user%" => "",
				  "Change username" => "",
				  "Change value" => "",
				  "Change values of all users in this domain" => "",
				  "Change webserver configuration" => "",
				  "City" => "",
				  "Clear session array" => "",
				  "Click here to start replication manually" => "",
				  "Clustering support" => "",
				  "Common name" => "",
				  "Completed" => "",
				  "Configuration" => "",
				  "Connected From" => "",
				  "Connection Number" => "",
				  "Connection ok, but could not bind to the directory" => "",
				  "Connections" => "",
				  "Contact person" => "",
				  "Contains" => "",
				  "Continue" => "",
				  "Control directory connection" => "",
				  "Control extension deactivated" => "",
				  "Control options" => "",
				  "Could not connect to port 389 at %host%, please make sure the service is up and it is not blocked with a firewall" => "",
				  "Could not connect to port 389 at %host%, please make sure the service is up and that it's not blocked with a firewall" => "",
				  "Could not figure out what objectclasses to add" => "",
				  "Create DNS template" => "",
				  "Create DNS template zone file" => "",
				  "Create a DNS zone in branch %domain%" => "",
				  "Create a webserver configuration in branch %domain%" => "",
				  "Create branches" => "",
				  "Create domain" => "",
				  "Create domain branch in LDAP server %server%" => "",
				  "Create list" => "",
				  "Create mailinglist" => "",
				  "Create mailinglist in domain %domain%" => "",
				  "Create mailserver install script" => "",
				  "Create new alias" => "",
				  "Create new forwarding address" => "",
				  "Create subbranch in domain %domain%" => "",
				  "Create unit" => "",
				  "Create user in domain %domain%" => "",
				  "Current LDAP Server Resource Usage" => "",
				  "Current Time on Server" => "",
				  "DN modification access - domain DN's" => "",
				  "DNS informations" => "",
				  "DNS zone" => "",
				  "Databases In Use" => "",
				  "Days" => "",
				  "Default" => "",
				  "Default (taken from ldapDefaultDotMode)" => "",
				  "Default domain" => "",
				  "Default domain for addresses without @" => "",
				  "Default domain name" => "",
				  "Default domain values" => "",
				  "Default domainname" => "",
				  "Default dot mode" => "",
				  "Default homedirectory for users" => "",
				  "Default host" => "",
				  "Default mailbox prefix path" => "",
				  "Default maildirectory for users" => "",
				  "Default password scheme" => "",
				  "Default properties" => "",
				  "Default quota" => "",
				  "Defined SMTP routes" => "",
				  "Defined in %attribute%" => "",
				  "Delay submitting after %value% RCPTO's" => "",
				  "Delete additional domain name" => "",
				  "Delete alias" => "",
				  "Delete all forwards" => "",
				  "Delete attribute" => "",
				  "Delete attribute %attribute% for %domainname%" => "",
				  "Delete host" => "",
				  "Delete it" => "",
				  "Delete list" => "",
				  "Delete mailserver control object" => "",
				  "Delete options" => "",
				  "Delete user" => "",
				  "Delete value" => "",
				  "Deliver locally if ldap didn't match a user" => "",
				  "Deliver mail to local mail box" => "",
				  "Delivery mode" => "",
				  "Delivery program" => "",
				  "Delivery program path" => "",
				  "Deliverymode" => "",
				  "Deny user admin access to domain" => "",
				  "Description" => "",
				  "Disable account" => "",
				  "Disable clustering" => "",
				  "Disable local lookup" => "",
				  "Disable rebind" => "",
				  "Disabled" => "",
				  "Disabled, don't rebind, retrieve password (Default)" => "",
				  "Disabled, single ldap server" => "",
				  "Disabled. Don't lookup for local users if ldap lookup didn't match" => "",
				  "Disallow user to administrate Controls information" => "",
				  "Disallow user to create domains below %domain%" => "",
				  "Do not allow program delivery (ignores deliveryProgramPath and .qmail)" => "",
				  "Do not save in local mailbox, allow forwarding and program delivery" => "",
				  "Document root" => "",
				  "Documentation" => "",
				  "Domain" => "",
				  "Domain %domain% successfully created" => "",
				  "Domain administrator" => "",
				  "Domain modification access" => "",
				  "Domain name" => "",
				  "Domain objectclasses" => "",
				  "Domainname" => "",
				  "Domains" => "",
				  "Donate it to another user" => "",
				  "Dot mode" => "",
				  "Dotmode" => "",
				  "Dotonly (only .qmail)" => "",
				  "Echo to console (tricky)" => "",
				  "Edit list owner address" => "",
				  "Email" => "",
				  "Empty field to set to the default value (%default% seconds)" => "",
				  "Enable clustering" => "",
				  "Enable local lookup" => "",
				  "Enable rebind" => "",
				  "Enabled, rebind with retrieved DN, don't retrieve password" => "",
				  "Enabled, use native clustering" => "",
				  "Enabled. Lookup for local users if ldap lookup didn't match" => "",
				  "Encrypted" => "",
				  "Ends with" => "",
				  "Entries Sent To Clients" => "",
				  "Entry Cache Usage" => "",
				  "Error" => "",
				  "Error logfile" => "",
				  "Expiration date" => "",
				  "Expire time" => "",
				  "External %what% creation script" => "",
				  "Extra List options" => "",
				  "Failed" => "",
				  "Failed adding %domain% to %value% %what%" => "",
				  "Failed running a test connection to %server%" => "",
				  "Failed sending mail" => "",
				  "Failed to %action% ACI to DN" => "",
				  "Failed to add %domain% to %what%" => "",
				  "Failed to add %what%" => "",
				  "Failed to add user %user% to group %group%<br>" => "",
				  "Failed to change %what%" => "",
				  "Failed to change %what% for domain %domain%" => "",
				  "Failed to change the default domainname" => "",
				  "Failed to create the domain" => "",
				  "Failed to create the new user" => "",
				  "Failed to create the sub unit" => "",
				  "Failed to remove %domain% from %what%" => "",
				  "Failed to remove %user% from %what%" => "",
				  "Failed to remove %value%" => "",
				  "Failed to remove the domain" => "",
				  "Failed to remove user" => "",
				  "Failed to removed alias %mail%" => "",
				  "Failed to rename RDN %user%" => "",
				  "Failed to replicate %what%" => "",
				  "Failed to set account expiration date" => "",
				  "Fax number" => "",
				  "Find user" => "",
				  "Find user(s) whose" => "",
				  "Forward" => "",
				  "Forward account" => "",
				  "Forwarders from other accounts to this user" => "",
				  "Forwarding account" => "",
				  "Forwarding address" => "",
				  "From address" => "",
				  "Give (more) admin rights" => "",
				  "Give admin rights" => "",
				  "Global configuration" => "",
				  "Help text taken from life with qmail-ldap" => "",
				  "Help text taken from qmail man pages" => "",
				  "Home" => "",
				  "Home directory" => "",
				  "Host" => "",
				  "Host address (FQDN)" => "",
				  "Host for bouncing" => "",
				  "Host name for Message-ID's" => "",
				  "Host(s)" => "",
				  "Hostmaster" => "",
				  "Hostname" => "",
				  "Hostname missing" => "",
				  "Hours" => "",
				  "IP Address" => "",
				  "If all left unset, no replication will take place" => "",
				  "If not mailbox in list directory" => "",
				  "If not same as listname" => "",
				  "If using {KERBEROS} as password scheme, make sure you include the correct REALM (principal@REALM.TLD)" => "",
				  "If you enter an already encrypted password, you must make sure that the password scheme you've choosen is the correct one. Also, choose the checkbox \uPassword is already encrypted\U" => "",
				  "If you set dotmode to both or ldapwithprog you should add a deliveryprogram too" => "",
				  "Include username in additional domains as alias" => "",
				  "Invalid" => "",
				  "Invalid category" => "",
				  "Invalid domain name! Use: domain.tld (e.g. adfinis.com)" => "",
				  "Is" => "",
				  "Keep old RDN" => "",
				  "Kerberos admin keytab" => "",
				  "Kerberos admin principal" => "",
				  "Kerberos admin server" => "",
				  "Kerberos realm" => "",
				  "Kilobytes Sent To Clients" => "",
				  "LDAP Attribute types" => "",
				  "LDAP Connection Status" => "",
				  "LDAP Database Status" => "",
				  "LDAP Matching rules" => "",
				  "LDAP Object classes" => "",
				  "LDAP Server" => "",
				  "LDAP Server Configuration" => "",
				  "LDAP Server Connection Status" => "",
				  "LDAP Server Database Status" => "",
				  "LDAP Server Resource Summary" => "",
				  "LDAP Server Startup Time" => "",
				  "LDAP Server Status" => "",
				  "LDAP Server Uptime" => "",
				  "LDAP Server Version" => "",
				  "LDAP Syntaxes" => "",
				  "LDAP attribute " => "",
				  "LDAP attribute " => "",
				  "LDAP base dn" => "",
				  "LDAP control base dn" => "",
				  "LDAP host" => "",
				  "LDAP server" => "",
				  "LDAP server connection and setup tests" => "",
				  "LDAP server connection option" => "",
				  "LDAP server contains phpQLAdminBranch objectclass" => "",
				  "LDAP server contains phpQLAdminConfig objectclass" => "",
				  "LDAPOnly (deliveryProgramPath + .qmail ignored for ldap deliveries)" => "",
				  "LDAPWithProg (deliveryProgramPath used, .qmail ignored)" => "",
				  "Language" => "",
				  "Language translator" => "",
				  "Lastname" => "",
				  "Leave the fields empty to set the default values" => "",
				  "Leave the fields empty to use default configuration" => "",
				  "Length of suffix" => "",
				  "Length of user suffix" => "",
				  "List" => "",
				  "List information" => "",
				  "List name" => "",
				  "List owner" => "",
				  "List owner address" => "",
				  "Local" => "",
				  "Lock POP account" => "",
				  "Locked" => "",
				  "Log out" => "",
				  "Login" => "",
				  "Login (valid dn)" => "",
				  "Login ID" => "",
				  "Login again" => "",
				  "Login shell" => "",
				  "MMaximum allowed mailinglists in domain %domain%" => "",
				  "MX Priority" => "",
				  "MX record" => "",
				  "Mail account" => "",
				  "Mail address" => "",
				  "Mail directory" => "",
				  "Mail exchanger" => "",
				  "Mail server" => "",
				  "Mailbox path" => "",
				  "Mailbox properties" => "",
				  "Mailhost" => "",
				  "Mailinglist administrator in branch" => "",
				  "Mailinglist administrators" => "",
				  "Mails" => "",
				  "Mailserver" => "",
				  "Main address" => "",
				  "Make the list a sublist of list" => "",
				  "Manage Controls DB" => "",
				  "Manage DNS domains" => "",
				  "Manage EZMLM mailinglists" => "",
				  "Manage webserver config" => "",
				  "Max %maxmails% mails" => "",
				  "Max %maxsize% KB" => "",
				  "Max lifetime of a message in queue" => "",
				  "Maximum" => "",
				  "Maximum RCPT TO's" => "",
				  "Maximum allowed mailinglists in branch" => "",
				  "Maximum allowed mailinglists in domain %domain%" => "",
				  "Maximum allowed users in branch" => "",
				  "Maximum amount of users reached" => "",
				  "Maximum number databytes in message" => "",
				  "Maximum number of RCPT TO's" => "",
				  "Maximum number of mails" => "",
				  "Maximum simultaneous deliveries" => "",
				  "Maximum size of mailbox" => "",
				  "Message" => "",
				  "Minimum GID Number to be used for shell and system accounts" => "",
				  "Minimum UID Number to be used for shell and system accounts" => "",
				  "Minutes" => "",
				  "Miscellaneous" => "",
				  "Missing" => "",
				  "Mobile" => "",
				  "Mode" => "",
				  "Modification access - phpQLAdmin configuration" => "",
				  "Modify %attribute% for %what%" => "",
				  "Modify SOA value" => "",
				  "Modify administrators for %domainname%" => "",
				  "Modify attribute %attribute% for %domainname%" => "",
				  "Modify contact persons for %domain%" => "",
				  "Modify host value" => "",
				  "Months" => "",
				  "Must be above zero" => "",
				  "Name" => "",
				  "Name server" => "",
				  "New RDN" => "",
				  "No" => "",
				  "No %attrib% defined" => "",
				  "No address defined" => "",
				  "No default" => "",
				  "No email address given" => "",
				  "No listname defined for list %listnr%" => "",
				  "No local mailbox" => "",
				  "No objectclasses" => "",
				  "No program deliveries" => "",
				  "No routes defined" => "",
				  "No users registred" => "",
				  "No webserver configurations in this branch" => "",
				  "No zones configure for this branch" => "",
				  "No. Reason:" => "",
				  "None" => "",
				  "None (deliveryProgramPath + .qmail ignored)" => "",
				  "Normal" => "",
				  "Not available" => "",
				  "Not set" => "",
				  "Numbers, letters and the following special chars: @, %, . (dot), _, -.\nIf left out, a username will be created automatically" => "",
				  "OID" => "",
				  "Object DN" => "",
				  "Object class violation" => "",
				  "Oid" => "",
				  "Old RDN" => "",
				  "Old address" => "",
				  "Old phpQLAdmin schema" => "",
				  "Old value" => "",
				  "Only forward" => "",
				  "Open Connections" => "",
				  "Operations Initiated" => "",
				  "Optional" => "",
				  "Options" => "",
				  "Organization name" => "",
				  "PHP LDAP extension" => "",
				  "POP locked" => "",
				  "POP status" => "",
				  "Pager" => "",
				  "Parent list" => "",
				  "Password" => "",
				  "Password encryption scheme" => "",
				  "Password is already encrypted" => "",
				  "Password scheme to use if not specifically specified" => "",
				  "Path" => "",
				  "Path to Kerberos \ukadmin\U command" => "",
				  "Path to homedir creating script" => "",
				  "Path to homedirectory" => "",
				  "Path to mailbox" => "",
				  "Permissions" => "",
				  "Personal details" => "",
				  "Please load file" => "",
				  "Please login" => "",
				  "Plus domain" => "",
				  "Port" => "",
				  "Post box" => "",
				  "Postal address" => "",
				  "Postal code" => "",
				  "Priority" => "",
				  "Private domain values" => "",
				  "Profile %type% account includes" => "",
				  "Program deliveries" => "",
				  "Put user in subbranch" => "",
				  "QMQP servers" => "",
				  "Rebind to LDAP server" => "",
				  "Received" => "",
				  "Receiver" => "",
				  "Record" => "",
				  "Record type missing" => "",
				  "Reference %what% with" => "",
				  "Refresh time" => "",
				  "Register new ACI" => "",
				  "Register new user" => "",
				  "Registred addresses" => "",
				  "Registred users" => "",
				  "Rejected address(es)" => "",
				  "Rejected local addresses" => "",
				  "Relay host" => "",
				  "Remain" => "",
				  "Remote" => "",
				  "Remote server" => "",
				  "Remove %domain% from %attribute%" => "",
				  "Remove DNS zone %domainname%" => "",
				  "Remove attribute %attribute% for user %user%" => "",
				  "Remove contact person from" => "",
				  "Remove domain" => "",
				  "Remove domain %domain%" => "",
				  "Remove domain administrator" => "",
				  "Remove the domain %domain%" => "",
				  "Remove this %what%" => "",
				  "Remove user %user% from domain %domain%" => "",
				  "Remove user from group" => "",
				  "Rename DN" => "",
				  "Rename this RDN" => "",
				  "Resource" => "",
				  "Resource destination missing" => "",
				  "Retry time" => "",
				  "Room number" => "",
				  "SMTP connection" => "",
				  "SMTP greeting message" => "",
				  "SMTP routes" => "",
				  "SOA Record" => "",
				  "SOA Value" => "",
				  "SOA value" => "",
				  "SRV" => "",
				  "Save" => "",
				  "Save in local mailbox" => "",
				  "Save in local mailbox only (no forwarding)" => "",
				  "Save in local mailbox, allow forwarding and program delivery" => "",
				  "Scheme" => "",
				  "Scope" => "",
				  "Script alias URL" => "",
				  "Script alias path" => "",
				  "Search" => "",
				  "Search Results" => "",
				  "Seconds" => "",
				  "Send an auto reply" => "",
				  "Send an automatic reply mail" => "",
				  "Send automatic reply" => "",
				  "Send quota warnings" => "",
				  "Send testmail" => "",
				  "Serial number" => "",
				  "Server Administrator" => "",
				  "Server IP" => "",
				  "Server IP[:PORT]" => "",
				  "Server URL" => "",
				  "Server administrator" => "",
				  "Server alias" => "",
				  "Server configuration DN" => "",
				  "Server connection options" => "",
				  "Set attribute %attribute%" => "",
				  "Set password" => "",
				  "Set tarpit count to 0 to disable tarpitting" => "",
				  "Set to 0 to allow unlimited message size" => "",
				  "Set, but now shown" => "",
				  "Shell account" => "",
				  "Show users" => "",
				  "Single valued" => "",
				  "Sorry, could not delete zone" => "",
				  "Sorry, no changes have been made to the mailbox. It's not implemented in phpQLAdmin yet" => "",
				  "Sorry, you do not have access to create mailinglists in this domain" => "",
				  "Sorry, you have reached the maximum allowed mailinglists in this domain" => "",
				  "Source domain" => "",
				  "Standard" => "",
				  "Standard (DNS entry)" => "",
				  "Starts with" => "",
				  "State" => "",
				  "Status" => "",
				  "Street address" => "",
				  "Sub unit %unit% successfully created" => "",
				  "Subject" => "",
				  "Subscriber" => "",
				  "Subscriber address(es)" => "",
				  "Subscription address" => "",
				  "Subscription options" => "",
				  "Subscription verification" => "",
				  "Substitution for local IP" => "",
				  "Successfully %action% ACI to DN" => "",
				  "Successfully added %domain% to %value %what%" => "",
				  "Successfully added %domain% to %what%" => "",
				  "Successfully added %what% %new%" => "",
				  "Successfully added user %user% to group %group%<br>" => "",
				  "Successfully changed %what%" => "",
				  "Successfully changed %what% for domain %domain%" => "",
				  "Successfully changed %what% from %old% to %new%" => "",
				  "Successfully changed %what% to %new%" => "",
				  "Successfully created the new user" => "",
				  "Successfully deleted %what%" => "",
				  "Successfully executed the %what% add script" => "",
				  "Successfully removed %domain% from %what%" => "",
				  "Successfully removed %user% from %what% %group%" => "",
				  "Successfully removed %value%" => "",
				  "Successfully removed DNS zone" => "",
				  "Successfully removed alias %mail%" => "",
				  "Successfully removed the domain" => "",
				  "Successfully removed user" => "",
				  "Successfully renamed RDN %user%" => "",
				  "Successfully replicated %what%" => "",
				  "Successfully sended mail to %email%" => "",
				  "Successfully set account expiration date" => "",
				  "Suffix" => "",
				  "Surname" => "",
				  "System account" => "",
				  "Taken from 'me'" => "",
				  "Target" => "",
				  "Tarpit count" => "",
				  "Tarpit delay" => "",
				  "Tarpitting settings" => "",
				  "Telefax number" => "",
				  "Telephone number" => "",
				  "Test LDAP-Connection" => "",
				  "Test connection after submitting form (it will be tested for each server seperatly)" => "",
				  "Text for replying" => "",
				  "Text in bold is a MUST, and non-bold is MAY" => "",
				  "The %what% add script failed" => "",
				  "The DN with the asterisk (*) is the DN where phpQLAdmin configurations are stored. If you add a user to that DN, the user will get 'Super Administration Rights'" => "",
				  "The automatic replication with domain is disabled" => "",
				  "The combination of user and host will return the email address used for bouning messages, e.g. MAILER-DAEMON@mail.host.ch" => "",
				  "The email address and the username will be automatically converted to lowercase" => "",
				  "The hostname %host% could not be resolved" => "",
				  "The qmaildotmode of this user is set to %qmaildotmode%. Please to set it to both or ldapWithProg to get the deliveryprogram running" => "",
				  "The username can only contain numbers, letters and following special characters @, . (dot), _, -. Additionally, the username should be unique in the system" => "",
				  "The username will be created with this as prefix, and a incrementing number after it when automaticly creating a username (uid)" => "",
				  "These are the objectclasses that exists in the LDAP server, exept those that's already included for use" => "",
				  "These must be a valid user ID and group ID for the system which is running qmail-ldap. Setting these to 0 (root) is a bad idea" => "",
				  "This affects LDAP delivery only. It doesn't work for local non-ldap deliveries" => "",
				  "This domain already exists" => "",
				  "This domain isn't registred within the DNS system" => "",
				  "This is a simple test to show if the ldap extension is loaded and that the connections are working. It also does some rudimentary ACL tests" => "",
				  "This is the length of the automaticly generated numbers after the prefix" => "",
				  "This sub unit already exists" => "",
				  "Time Opened" => "",
				  "Time To Live" => "",
				  "Time when the account is disabled" => "",
				  "Timeouts" => "",
				  "Timeoutsmtpd (How long to wait for SMTP client)" => "",
				  "Title" => "",
				  "To block a whole domain simply leave user empty (eg. @domain.com). To delete a blocked address remove the value and save" => "",
				  "To delete a QMQP server remove the value and save" => "",
				  "To delete a domain, remove the value and save" => "",
				  "To delete an address remove the value and save" => "",
				  "Toggle" => "",
				  "Transfer logfile" => "",
				  "Type" => "",
				  "UID;GID" => "",
				  "Unit name" => "",
				  "Unknown action %action% in file %file%, function save()" => "",
				  "Unknown attribute %attribute% in %file%" => "",
				  "Unknown attribute value %value% for %attribute%" => "",
				  "Unknown save type %type% in file %file%, function save()" => "",
				  "Unlimited" => "",
				  "Unlimited amount" => "",
				  "Unlimited size" => "",
				  "Unsubscribe user from all mailing lists" => "",
				  "Unsubscription verification" => "",
				  "Usage Since Startup" => "",
				  "Use additional message" => "",
				  "Use login and password" => "",
				  "User" => "",
				  "User %user% already exists" => "",
				  "User %user% does not exist" => "",
				  "User DN" => "",
				  "User access" => "",
				  "User data" => "",
				  "User defined" => "",
				  "User directory connection" => "",
				  "User for bouncing" => "",
				  "User objectclasses" => "",
				  "Username" => "",
				  "Username prefix" => "",
				  "Username prefix to use when automaticly creating username in domain %domain%" => "",
				  "Using domain mode (dc separated). Don't use '.' (dots) in domain name" => "",
				  "Using organization layout (o separated)" => "",
				  "Using organizationUnit layout (ou separated). Make sure you're using 'domain.tld' for domain name" => "",
				  "VAT number" => "",
				  "Value" => "",
				  "Value in 'me'" => "",
				  "Wait" => "",
				  "Webserver administration" => "",
				  "Weeks" => "",
				  "Welcome to \b%whoarewe%\B" => "",
				  "What should we do with forwards to this user" => "",
				  "What should we do with the users mailbox" => "",
				  "What's been done" => "",
				  "What's left todo" => "",
				  "Who are we" => "",
				  "With option DNS, the first MX record of this domain will be taken" => "",
				  "Work" => "",
				  "Yes" => "",
				  "You can add more forwarding address in the user details page" => "",
				  "You have %count% mailinglists, but only %allowed% is allowed." => "",
				  "You have to provide a value to search for" => "",
				  "You have to set both, host and base dn to make qmail-ldap work" => "",
				  "You must select at least one type of quota" => "",
				  "\uA\Udvanced mode" => "",
				  "accountstatus" => "",
				  "additional address" => "",
				  "additional domain" => "",
				  "additional domain name" => "",
				  "additional group" => "",
				  "alias" => "",
				  "badmailfrom options" => "",
				  "badrcptto options" => "",
				  "bouncing options" => "",
				  "can't autogenerate" => "",
				  "contact person" => "",
				  "contact person for domain" => "",
				  "custombouncetext message" => "",
				  "databytes options" => "",
				  "default domain name" => "",
				  "default homedirectory base" => "",
				  "default maildirectory base" => "",
				  "default password scheme" => "",
				  "default quota" => "",
				  "default values" => "",
				  "delivery mode" => "",
				  "dirmaker" => "",
				  "dirmaker options" => "",
				  "domain" => "",
				  "domain administrator" => "",
				  "domain administrator for domain" => "",
				  "domain branch" => "",
				  "domain objectClass" => "",
				  "domain to %what% in following objects" => "",
				  "domain to '.pql_get_define(" => "",
				  "domain to '.pql_get_define('PQL_ATTR_RCPTHOSTS" => "",
				  "domain to branch" => "",
				  "domains" => "",
				  "dot mode" => "",
				  "double bouncing options" => "",
				  "email address" => "",
				  "envnoathost options" => "",
				  "envnotathost options" => "",
				  "forward" => "",
				  "forward only, no local mailbox" => "",
				  "forwarding address" => "",
				  "helohost options" => "",
				  "host" => "",
				  "idhost options" => "",
				  "ldapcluster options" => "",
				  "ldapdefaultdotmode options" => "",
				  "ldaplocaldelivery options" => "",
				  "ldaprebind options" => "",
				  "ldapserver options" => "",
				  "local delivery holded" => "",
				  "local mailbox, POP account" => "",
				  "localiphost options" => "",
				  "locals" => "",
				  "login shell" => "",
				  "loginshell, homedirectory" => "",
				  "loginshell, homedirectory, no mail etc" => "",
				  "mail" => "",
				  "mailbox path" => "",
				  "mailbox path " => "",
				  "mailinglist administrator" => "",
				  "mailinglist administrator for domain" => "",
				  "mailserver" => "",
				  "main address" => "",
				  "maximum allowed mailinglists" => "",
				  "maximum allowed users" => "",
				  "maxrcptcount options" => "",
				  "minutes" => "",
				  "more servers" => "",
				  "number to be used for forwarding accounts" => "",
				  "only .qmail" => "",
				  "password" => "",
				  "password encryption scheme" => "",
				  "path" => "",
				  "percenthack options" => "",
				  "phpQLAdmin Configuration" => "",
				  "phpQLAdmin Site Specifics" => "",
				  "phpQLAdmin configuration" => "",
				  "phpQLAdmin ldap connection" => "",
				  "qmqpservers options" => "",
				  "queuelifetime options" => "",
				  "quota" => "",
				  "quota count" => "",
				  "quota size" => "",
				  "quotawarning message" => "",
				  "rcpthosts" => "",
				  "relaymailfrom options" => "",
				  "remote delivery holded" => "",
				  "remote timeout options" => "",
				  "server alias" => "",
				  "server not responding or invalid name" => "",
				  "server option" => "",
				  "shell" => "",
				  "smtpgreeting options" => "",
				  "smtproutes options" => "",
				  "sub unit" => "",
				  "subscriber" => "",
				  "successfully changed %what%" => "",
				  "system" => "",
				  "tarpitting options" => "",
				  "timeoutsmtpd options" => "",
				  "to all hosts" => "",
				  "user" => "",
				  "user objectClass" => "",
				  "username" => "",
				  "username prefix" => "",
				  "users" => "",
				  "virtual user options" => "",
				  "virtual user settings" => "",
				  "webserver configuration" => "",
				  "xxx options" => "");

/*
 * Local variables:
 * mode: php
 * tab-width: 4
 * End:
 */
?>