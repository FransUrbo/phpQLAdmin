<?php
// $Id: texts.radius.inc,v 1.2 2007-09-29 21:08:10 turbo Exp $

// {{{ Simplicity - with this we can have ONE single foreach() loop
// to output the whole view!
// Idea taken from tables/config_details-ppolicy.inc.
$table_columns = array(pql_get_define("PQL_ATTR_RADIUS_ARAP_FEATURES")				=> $LANG->_('ARAP "Feature flags" package information'),
					   pql_get_define("PQL_ATTR_RADIUS_ARAP_SECURITY")				=> $LANG->_('What ARAP Security module to use in an Access-Challenge packet.'),
					   pql_get_define("PQL_ATTR_RADIUS_ARAP_ZONE_ACCESS")			=> $LANG->_('How to use a ARAP zone list'),
					   pql_get_define("PQL_ATTR_RADIUS_AUTH_TYPE")					=> $LANG->_('Authentication module to call'),
					   pql_get_define("PQL_ATTR_RADIUS_CALLBACK_ID")				=> $LANG->_('The name of a place to be called'),
					   pql_get_define("PQL_ATTR_RADIUS_CALLBACK_NR")				=> $LANG->_('The number to which the RADIUS client gear should return a call to the authenticating user'),
					   pql_get_define("PQL_ATTR_RADIUS_CALLED_STATION_ID")			=> $LANG->_('Allows the NAS to send (in the Access-Request package) the phone number (or MAC address) the user called'),
					   pql_get_define("PQL_ATTR_RADIUS_CALLING_STATION_ID")			=> $LANG->_('Allows the NAS to send (in the Access-Request package) the phone number (or MAC address) the call came from'),
					   pql_get_define("PQL_ATTR_RADIUS_CLASS")						=> $LANG->_('Funnel identification and property information of the accounting systems of RADIUS implementations'),
					   pql_get_define("PQL_ATTR_RADIUS_CLIENT_IP")					=> $LANG->_('Client IP address'),
					   pql_get_define("PQL_ATTR_RADIUS_FILTER_ID")					=> $LANG->_('Name of the filter list for this user'),
					   pql_get_define("PQL_ATTR_RADIUS_FRAMED_APPLETALK_LINK")		=> $LANG->_('AppleTalk network number'),
					   pql_get_define("PQL_ATTR_RADIUS_FRAMED_APPLETALK_NETW")		=> $LANG->_('AppleTalk Network number'),
					   pql_get_define("PQL_ATTR_RADIUS_FRAMED_APPLETALK_ZONE")		=> $LANG->_('AppleTalk Default Zone'),
					   pql_get_define("PQL_ATTR_RADIUS_FRAMED_COMPRESSION")			=> $LANG->_('Compression protocol to use'),
					   pql_get_define("PQL_ATTR_RADIUS_FRAMED_IP_ADDR")				=> $LANG->_('Address to be configured for the user'),
					   pql_get_define("PQL_ATTR_RADIUS_FRAMED_IP_NET")				=> $LANG->_('IP netmask to be configured for the user\n(when the user is a router to a network)'),
					   pql_get_define("PQL_ATTR_RADIUS_FRAMED_IPX_NET")				=> $LANG->_('IPX Network number to be configured for the user'),
					   pql_get_define("PQL_ATTR_RADIUS_FRAMED_MTU")					=> $LANG->_('Maximum Transmission Unit to be configured for the user'),
					   pql_get_define("PQL_ATTR_RADIUS_FRAMED_PROTO")				=> $LANG->_('Framing to be used for framed access'),
					   pql_get_define("PQL_ATTR_RADIUS_FRAMED_ROUTE")				=> $LANG->_('Routing information to be configured for the user on the NAS'),
					   pql_get_define("PQL_ATTR_RADIUS_FRAMED_ROUTING")				=> $LANG->_('Routing method for the user\n(when the user is a router to a network)'),
					   pql_get_define("PQL_ATTR_RADIUS_GROUP_NAME")					=> $LANG->_('LDAP Group name for access'),
					   pql_get_define("PQL_ATTR_RADIUS_HINT")						=> $LANG->_('Hint value'),
					   pql_get_define("PQL_ATTR_RADIUS_HUNTGROUP_NAME")				=> $LANG->_('[radiusHuntgroupName]'),
					   pql_get_define("PQL_ATTR_RADIUS_IDLE_TIMEOUT")				=> $LANG->_('Max concecutive seconds of idle connection allowed'),
					   pql_get_define("PQL_ATTR_RADIUS_LOGIN_IP_HOST")				=> $LANG->_('System with which to connect the user when loged in'),
					   pql_get_define("PQL_ATTR_RADIUS_LOGIN_LAT_GROUP")			=> $LANG->_('LAT group codes which this user is authorized to use'),
					   pql_get_define("PQL_ATTR_RADIUS_LOGIN_LAT_NODE")				=> $LANG->_('Node to which the user is to be automatically connected by LAT'),
					   pql_get_define("PQL_ATTR_RADIUS_LOGIN_LAT_PORT")				=> $LANG->_('Port to which the user is to be connected by LAT'),
					   pql_get_define("PQL_ATTR_RADIUS_LOGIN_LAT_SERVICE")			=> $LANG->_('System to which the user is to be connected by LAT'),
					   pql_get_define("PQL_ATTR_RADIUS_LOGIN_SERVICE")				=> $LANG->_('Service which should be used to connect the user to the login host'),
					   pql_get_define("PQL_ATTR_RADIUS_LOGIN_TCP_PORT")				=> $LANG->_('TCP port of login service'),
					   pql_get_define("PQL_ATTR_RADIUS_LOGIN_TIME")					=> $LANG->_('Defines the time span a user may login to the system'),
					   pql_get_define("PQL_ATTR_RADIUS_PASSWORD_RETRY")				=> $LANG->_('How many authentication attempts a user may be allowed to attempt before being disconnected'),
					   pql_get_define("PQL_ATTR_RADIUS_PORT_LIMIT")					=> $LANG->_('Set the maximum number of ports to be provided to the user by the NAS'),
					   pql_get_define("PQL_ATTR_RADIUS_PROFILE_DN")					=> $LANG->_('RADIUS profile object to use'),
					   pql_get_define("PQL_ATTR_RADIUS_PROMPT")						=> $LANG->_('[radiusPrompt]'),
					   pql_get_define("PQL_ATTR_RADIUS_PROXY2REALM")				=> $LANG->_('[radiusProxyToRealm]'),
					   pql_get_define("PQL_ATTR_RADIUS_REPLY_MESSAGE")				=> $LANG->_('[radiusReplyMessage]'),
					   pql_get_define("PQL_ATTR_RADIUS_REALM")						=> $LANG->_('User realm'),
					   pql_get_define("PQL_ATTR_RADIUS_SERVICE_TYPE")				=> $LANG->_('Type of service the user has requested\n(or the type of service to be provided)'),
					   pql_get_define("PQL_ATTR_RADIUS_SESSION_TIMEOUT")			=> $LANG->_('Max number of seconds of service to be provided to the user before termination of the session or prompt'),
					   pql_get_define("PQL_ATTR_RADIUS_TERMINATE_ACTION")			=> $LANG->_('What action the NAS should take when the specified service is completed'),
					   pql_get_define("PQL_ATTR_RADIUS_TUNNEL_ASSIGN_ID")			=> $LANG->_('What tunnel should be assigned to a session'),
					   pql_get_define("PQL_ATTR_RADIUS_TUNNEL_MEDIUM_TYPE")			=> $LANG->_('What transport medium to use when creating a tunnel'),
					   pql_get_define("PQL_ATTR_RADIUS_TUNNEL_PASSWORD")			=> $LANG->_('May contain a password to be used to authenticate to a remote server'),
					   pql_get_define("PQL_ATTR_RADIUS_TUNNEL_PREFS")				=> $LANG->_('What transport medium to use when creating a tunnel for those protocols (such as L2TP) that can operate over multiple transports'),
					   pql_get_define("PQL_ATTR_RADIUS_TUNNEL_PRIVATE_GROUP_ID")	=> $LANG->_('Group ID for a particular tunneled session'),
					   pql_get_define("PQL_ATTR_RADIUS_TUNNEL_SERVER_ENDPOINT")		=> $LANG->_('Address of the server end of the tunnel'),
					   pql_get_define("PQL_ATTR_RADIUS_TUNNEL_CLIENT_ENDPOINT")		=> $LANG->_('Address of the initiator end of the tunnel'),
					   pql_get_define("PQL_ATTR_RADIUS_TUNNEL_TYPE")				=> $LANG->_('Tunneling protocol(s) in use'),
					   pql_get_define("PQL_ATTR_RADIUS_VSA")						=> $LANG->_('[radiusVSA]'),
					   pql_get_define("PQL_ATTR_RADIUS_SIMULT_USE")					=> $LANG->_('Maximum number of concurrent logins'),
					   pql_get_define("PQL_ATTR_RADIUS_USER_CATEGORY")				=> $LANG->_('Address of the server end of the tunnel'),
					   pql_get_define("PQL_ATTR_RADIUS_STRIP_USER_NAME")			=> $LANG->_('[radiusStripUserName]'),
					   pql_get_define("PQL_ATTR_RADIUS_DIALUP_ACCESS")				=> $LANG->_('Allow user dialup'),
					   pql_get_define("PQL_ATTR_RADIUS_EXPIRATION")					=> $LANG->_('User expiration date (DD Month YYYY)'));
// }}}

// {{{ Attributes <=> possible values mapping
$table_options = array("PQL_ATTR_RADIUS_AUTH_TYPE"			=> array("PAP", "CHAP", "MSCHAP", "PAM", "UNIX", "LDAP", "EAP"),
					   "PQL_ATTR_RADIUS_LOGIN_TIME"			=> array("Mo", "Tu", "We", "Th", "Fr", "Sa", "Su", "Wk", "All"));
// }}}

// {{{ Attributes <=> help texts
$table_helps = array("PQL_ATTR_RADIUS_FRAMED_APPLETALK_LINK" => "AppleTalk network number which should be used for the serial link to the user, which is another AppleTalk router.",
					 "PQL_ATTR_RADIUS_FRAMED_APPLETALK_NETW" => "AppleTalk Network number which the NAS should probe to allocate an AppleTalk node for the use");
// }}}

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
