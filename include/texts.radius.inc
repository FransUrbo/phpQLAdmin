<?php
// $Id: texts.radius.inc,v 1.1 2007-09-20 15:15:46 turbo Exp $

// {{{ Simplicity - with this we can have ONE single foreach() loop
// to output the whole view!
// Idea taken from tables/config_details-ppolicy.inc.
$table_columns = array(pql_get_define("PQL_ATTR_RADIUS_ARAP_FEATURES")				=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_ARAP_SECURITY")				=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_ARAP_ZONE_ACCESS")			=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_AUTH_TYPE")					=> $LANG->_('Authentication module to call'),
					   pql_get_define("PQL_ATTR_RADIUS_CALLBACK_ID")				=> $LANG->_('The name of a place to be called'),
					   pql_get_define("PQL_ATTR_RADIUS_CALLBACK_NR")				=> $LANG->_('The number to which the RADIUS client gear should return a call to the authenticating user'),
					   pql_get_define("PQL_ATTR_RADIUS_CALLED_STATION_ID")			=> $LANG->_('Allows the NAS to send (in the Access-Request package) the phone number (or MAC address) the user called'),
					   pql_get_define("PQL_ATTR_RADIUS_CALLING_STATION_ID")			=> $LANG->_('Allows the NAS to send (in the Access-Request package) the phone number (or MAC address) the call came from'),
					   pql_get_define("PQL_ATTR_RADIUS_CLASS")						=> $LANG->_('Funnel identification and property information of the accounting systems of RADIUS implementations'),
					   pql_get_define("PQL_ATTR_RADIUS_CLIENT_IP")					=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_FILTER_ID")					=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_FRAMED_APPLETALK_LINK")		=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_FRAMED_APPLETALK_NETW")		=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_FRAMED_APPLETALK_ZONE")		=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_FRAMED_COMPRESSION")			=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_FRAMED_IP_ADDR")				=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_FRAMED_IP_NET")				=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_FRAMED_IPX_NET")				=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_FRAMED_MTU")					=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_FRAMED_PROTO")				=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_FRAMED_ROUTE")				=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_FRAMED_ROUTING")				=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_GROUP_NAME")					=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_HINT")						=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_HUNTGROUP_NAME")				=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_IDLE_TIMEOUT")				=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_LOGIN_IP_HOST")				=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_LOGIN_LAT_GROUP")			=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_LOGIN_LAT_NODE")				=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_LOGIN_LAT_PORT")				=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_LOGIN_LAT_SERVICE")			=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_LOGIN_SERVICE")				=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_LOGIN_TCP_PORT")				=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_LOGIN_TIME")					=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_PASSWORD_RETRY")				=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_PORT_LIMIT")					=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_PROFILE_DN")					=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_PROMPT")						=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_PROXY2REALM")				=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_REPLICATE2REALM")			=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_REPLY_MESSAGE")				=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_REALM")						=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_SERVICE_TYPE")				=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_SESSION_TIMEOUT")			=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_TERMINATE_ACTION")			=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_TUNNEL_ASSIGN_ID")			=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_TUNNEL_MEDIUM_TYPE")			=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_TUNNEL_PASSWORD")			=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_TUNNEL_PREFS")				=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_TUNNEL_PRIVATE_GROUP_ID")	=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_TUNNEL_SERVER_ENDPOINT")		=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_TUNNEL_CLIENT_ENDPOINT")		=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_TUNNEL_TYPE")				=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_VSA")						=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_SIMULT_USE")					=> $LANG->_('Maximum number of concurrent logins'),
					   pql_get_define("PQL_ATTR_RADIUS_USER_CATEGORY")				=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_STRIP_USER_NAME")			=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_DIALUP_ACCESS")				=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_EXPIRATION")					=> $LANG->_('User expiration date (DD Month YYYY)'));
// }}}

// {{{ Attributes <=> possible values mapping
$table_options = array("PQL_ATTR_RADIUS_AUTH_TYPE"			=> array("PAP", "CHAP", "MSCHAP", "PAM", "UNIX", "LDAP", "EAP"));
// }}}

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
