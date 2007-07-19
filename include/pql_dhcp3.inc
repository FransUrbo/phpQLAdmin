<?php
// ----------------------------
// pql_dhcp3.inc
// phpQLAdmin Application Programming Interface (API)
// (special functions for DHCP3 server configuration)
//
// $Id: pql_dhcp3.inc,v 2.1 2007-07-19 10:28:01 turbo Exp $
//

// {{{ pql_dhcp3_get_subnets(dn)
function pql_dhcp3_get_subnets($dn) {
  global $_pql;

  // Setup the object class filter
  // (&(objectClass=dhcpSubnet)(|(dhcpSubnet=*)(dhcpNetMask=*)))
  $filter  = '(&';
  $filter .=   '('.pql_get_define("PQL_ATTR_OBJECTCLASS").'='.pql_get_define("PQL_ATTR_DHCP3_SUBNET").')';
  $filter .=   '(|';
  $filter .=      '('.pql_get_define("PQL_ATTR_DHCP3_RANGE").'=*)';
  $filter .=      '('.pql_get_define("PQL_ATTR_DHCP3_NETMASK").'=*)';
  $filter .=   ')';
  $filter .= ')';

  $result = $_pql->search($dn, $filter, "ONELEVEL");
  if(!$result[0]) { $result = array($result); }
  return($result);
}
// }}}

// {{{ pql_dhcp3_get_subhosts(dn)
function pql_dhcp3_get_subhosts($dn) {
  global $_pql;

  // Setup the object class filter
  // (&(objectClass=dhcpSubnet)(|(dhcpSubnet=*)(dhcpNetMask=*)))
  $filter = '('.pql_get_define("PQL_ATTR_OBJECTCLASS").'='.pql_get_define("PQL_ATTR_DHCP3_HOST").')';

  $result = $_pql->search($dn, $filter, "ONELEVEL");
  if(!$result[0]) { $result = array($result); }
  return($result);
}
// }}}


// {{{ pql_dhcp3_get_service_options(dn)
function pql_dhcp3_get_service_options($dn) {
  global $_pql;

  // dhcpPrimaryDN $ dhcpSecondaryDN $ dhcpServerDN $ dhcpSharedNetworkDN $ dhcpSubnetDN $
  // dhcpGroupDN $ dhcpHostDN $  dhcpClassesDN $ dhcpOptionsDN $ dhcpZoneDN $ dhcpKeyDN $
  // dhcpFailOverPeerDN $ dhcpStatements $ dhcpComments $ dhcpOption
  $attribs = array('statements'		=> pql_get_define("PQL_ATTR_DHCP3_STATEMENTS"),
				   'option'			=> pql_get_define("PQL_ATTR_DHCP3_OPTION"),
				   'comments'		=> pql_get_define("PQL_ATTR_DHCP3_COMMENTS"),

				   'primarydn'		=> pql_get_define("PQL_ATTR_DHCP3_PRIMARYDN"),
				   'secondarydn'	=> pql_get_define("PQL_ATTR_DHCP3_SECONDARYDN"),
				   'serverdn'		=> pql_get_define("PQL_ATTR_DHCP3_SERVERDN"),
				   'sharednetdn'	=> pql_get_define("PQL_ATTR_DHCP3_SHAREDNETWORKDN"),
				   'subnetdn'		=> pql_get_define("PQL_ATTR_DHCP3_SUBNETDN"),
				   'groupdn'		=> pql_get_define("PQL_ATTR_DHCP3_GROUPDN"),
				   'hostdn'			=> pql_get_define("PQL_ATTR_DHCP3_HOSTDN"),
				   'classesdn'		=> pql_get_define("PQL_ATTR_DHCP3_SUBCLASSESDN"),
				   'optionsdn'		=> pql_get_define("PQL_ATTR_DHCP3_OPTIONSDN"),
				   'zonedn'			=> pql_get_define("PQL_ATTR_DHCP3_ZONEDN"),
				   'keydn'			=> pql_get_define("PQL_ATTR_DHCP3_KEYDN"),
				   'failoverpeerdn'	=> pql_get_define("PQL_ATTR_DHCP3_FAIL_OVER_PEERDN"));
  foreach($attribs as $key => $attrib) {
	$values[$key] = $_pql->get_attribute($dn, $attrib);
  }

  return($values);
}
// }}}

// {{{ pql_dhcp3_get_subnet_options(dn)
function pql_dhcp3_get_subnet_options($dn) {
  global $_pql;

  // dhcpRange $ dhcpPoolDN $ dhcpGroupDN $ dhcpHostDN $ dhcpClassesDN $
  // dhcpLeasesDN $ dhcpOptionsDN $ dhcpZoneDN $ dhcpKeyDN $ dhcpFailOverPeerDN $
  // dhcpStatements $ dhcpComments $ dhcpOption
  $attribs = array('netmask'		=> pql_get_define("PQL_ATTR_DHCP3_NETMASK"),
				   'range'			=> pql_get_define("PQL_ATTR_DHCP3_RANGE"),
				   'option'			=> pql_get_define("PQL_ATTR_DHCP3_OPTION"),
				   'statements'		=> pql_get_define("PQL_ATTR_DHCP3_STATEMENTS"),
				   'comments'		=> pql_get_define("PQL_ATTR_DHCP3_COMMENTS"),

				   'pooldn'			=> pql_get_define("PQL_ATTR_DHCP3_POOLDN"),
				   'groupdn'		=> pql_get_define("PQL_ATTR_DHCP3_GROUPDN"),
				   'hostdn'			=> pql_get_define("PQL_ATTR_DHCP3_HOSTDN"),
				   'classesdn'		=> pql_get_define("PQL_ATTR_DHCP3_CLASSESDN"),
				   'leasesdn'		=> pql_get_define("PQL_ATTR_DHCP3_LEASESDN"),
				   'optionsdn'		=> pql_get_define("PQL_ATTR_DHCP3_ZONEDN"),
				   'keydn'			=> pql_get_define("PQL_ATTR_DHCP3_KEYDN"),
				   'failoverpeerdn'	=> pql_get_define("PQL_ATTR_DHCP3_FAIL_OVER_PEERDN"));
  foreach($attribs as $key => $attrib) {
	$values[$key] = $_pql->get_attribute($dn, $attrib);
  }

  return($values);
}
// }}}

// {{{ pql_dhcp3_get_subhost_options(dn)
function pql_dhcp3_get_subhost_options($dn) {
  global $_pql;

  // dhcpLeaseDN $ dhcpHWAddress $ dhcpOptionsDN $ dhcpStatements $ dhcpComments $ dhcpOption
  $attribs = array('hwaddr'			=> pql_get_define("PQL_ATTR_DHCP3_HW_ADDRESS"),
				   'statements'		=> pql_get_define("PQL_ATTR_DHCP3_STATEMENTS"),
				   'option'			=> pql_get_define("PQL_ATTR_DHCP3_OPTION"),
				   'comments'		=> pql_get_define("PQL_ATTR_DHCP3_COMMENTS"),

				   'leasedn'		=> pql_get_define("PQL_ATTR_DHCP3_LEASEDN"),
				   'optionsdn'		=> pql_get_define("PQL_ATTR_DHCP3_OPTIONSDN"));
  foreach($attribs as $key => $attrib) {
	$values[$key] = $_pql->get_attribute($dn, $attrib);
  }

  return($values);
}
// }}}

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
