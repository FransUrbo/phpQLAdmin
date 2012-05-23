<?php
// ----------------------------
// pql_search.inc
// phpQLAdmin Application Programming Interface (API)
//
// $Id: pql_search.inc,v 2.74 2008-03-18 22:34:24 turbo Exp $
//

// ------------------------------------------------
// Support SEARCH API functions
// ------------------------------------------------

// {{{ pql_get_domains()
// Get all domains listed in ldap-tree (ou|dc-records)
function pql_get_domains() {
  global $_pql, $_SESSION;

  $domains = array();
  
  foreach($_SESSION["BASE_DN"] as $TOP_DN)  {
	// BUG: I'd really like to use this, but I can't.
	// The reason is: If I specify (in 'phpQLAdmin config->Branch config')
	// to use the following object classeses when creating branches,
	// AND all those object classes doesn't already exists in the branch
	// object(s), no branches will be found!
	//$ocs = '(objectClass=phpQLAdminBranch)(objectClass=phpQLAdminInfo)'
	// But only the first (or none of them) exists -> no branches!!
	//$ocs = pql_setup_branch_objectclasses(1, $TOP_DN);
	
	if(pql_get_define("PQL_CONF_SUBTREE_USERS")) {
	  // There's a subtree defined - look if it's there
	  $dn = pql_get_define("PQL_CONF_SUBTREE_USERS").",$TOP_DN";
	  
	  $filter = pql_get_define("PQL_ATTR_OBJECTCLASS").'=*';
	  $result = $_pql->get_dn($dn, $filter, 'BASE');
	  if(file_exists($_SESSION["path"]."/.DEBUG_DOMAINS")) {
		echo "pql_get_domains(): result";
		printr($result);
	  }

	  if(is_array($result)) {
		// The PQL_CONF_SUBTREE_USERS,TOP_DN exists
		
		// Is there any users just below that?
		$filter  = pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $TOP_DN)."=*";
		$new = $_pql->get_dn($dn, $filter, 'ONELEVEL');
		if(file_exists($_SESSION["path"]."/.DEBUG_DOMAINS")) {
		  echo "pql_get_domains(): new(1)";
		  printr($new);
		}

		if(is_array($new)) {
		  // Yes, there are users here. Add this DN to the domains list
		  pql_add2array($domains, $TOP_DN);
		  
		  // Just to be safe - is there any branches here?
		  $filter = "(&(" . pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $TOP_DN) . "=*)$ocs)";
		  $new    = $_pql->get_dn($dn, $filter, 'ONELEVEL');
		  if(file_exists($_SESSION["path"]."/.DEBUG_DOMAINS")) {
			echo "pql_get_domains(): new (2)";
			printr($new);
		  }

		  if(is_array($new))
			pql_add2array($domains, $new);
		}
	  }
	}

	// else/and:

	// -------------------
	// Is there any users just below the top DN then?
	
	// Can't use $ocs (see above), so instead, I'll have to 'fake' it here.
	if(pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $TOP_DN) == 'o')
	  $ocs = '(objectClass=organization)';
	else if(pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $TOP_DN) == 'dc')
	  $ocs = '(objectClass=domain)'; // Could also be 'domainComponent'
	
	if(pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $TOP_DN))
	  $filter = pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $TOP_DN)."=*";
	else
	  // User reference for this branch/domain not specified. Use default...
	  $filter = 'uid=*';
	$new    = $_pql->get_dn($TOP_DN, $filter, 'ONELEVEL');
	if(file_exists($_SESSION["path"]."/.DEBUG_DOMAINS")) {
	  echo "pql_get_domains(): new(3)";
	  printr($new);
	}

	if(is_array($new))
	  pql_add2array($domains, $new);
	
	// Just to be safe - is there any branches here?
	$filter = "(&(" . pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $TOP_DN) . "=*)$ocs)";
	$new    = $_pql->get_dn($TOP_DN, $filter, 'ONELEVEL');
	if(file_exists($_SESSION["path"]."/.DEBUG_DOMAINS")) {
	  echo "pql_get_domains(): new(4)";
	  printr($new);
	}

	if(is_array($new))
	  pql_add2array($domains, $new);

	// Now, if the base/root DN is a 'dc' object and domain/branch reference
	// is 'ou', we MUST use 'organizationalUnit' in the object class filter...
	$filter = '(&('.pql_get_define("PQL_ATTR_OU").'=*)('.pql_get_define("PQL_ATTR_OBJECTCLASS").'=organizationalUnit))';
	$new    = pql_unit_get($TOP_DN, $filter);
	if(file_exists($_SESSION["path"]."/.DEBUG_DOMAINS")) {
	  echo "pql_get_domains(): new(5)";
	  printr($new);
	}

	if(is_array($new)) {
	  for($i=0; $new[$i]; $i++)
		pql_add2array($domains, $new);
	}
  }
  
  if($domains[0]) {
	// Sort the domain array by extracting the actual domain/branch name
	// and sort by that, not the DN ('ou=', 'o=', 'dc=' etc).
	for($i=0; $i < count($domains); $i++) {
	  // o=Bayour.COM,c=SE => Get 'Bayour.COM'
	  $tmp = explode(',', $domains[$i]);
	  $tmp = explode('=', $tmp[0]);
	  $tmp = $tmp[1];
	  
	  $dom[$domains[$i]] = lc($tmp);
	}
	
	// The array now looks like:
	// o=Bayour.COM,c=SE => bayour.com
	
	// Sort by _value_ (which is lower cased).
	if(defined("SORT_LOCALE_STRING"))
	  // Only availible in PHP >4.4.0
	  asort($dom, SORT_LOCALE_STRING);
	else
	  asort($dom);
	
	// Move the new array (which is correctly sorted) to the
	// original array.
	unset($tmp); // I'm reusing this variable below...
	foreach($dom as $dn => $value) {
	  for($i=0; $i < count($domains); $i++) {
		if((lc($dn) == lc($domains[$i])) and !preg_match('/^ou=Computers/i,', lc($dn)))
		  $tmp[] = $dn;
	  }
	}
	$domains = $tmp;
	
	return pql_uniq($domains);
  } else
	return false;
}
// }}}

// {{{ pql_get_branch_domains()
function pql_get_branch_domains() {
  global $_pql;

  // Get all branches in the database
  $branches = pql_get_domains();
  foreach($branches as $branch) {
	// {{{ Get default domain name for this domain/branch
	$default_domain = $_pql->get_attribute($branch, pql_get_define("PQL_ATTR_DEFAULTDOMAIN"));
	if($default_domain) {
	  if(!is_array($default_domain))
		$default_domain = array($default_domain);
	  
	  pql_add2array($domain_names, $default_domain);
	}
	// }}}
	
	// {{{ Get additional domain name(s)
	$additional = $_pql->get_attribute($branch, pql_get_define("PQL_ATTR_ADDITIONAL_DOMAINNAME"));
	if($additional) {
	  if(!is_array($additional))
		$additional = array($additional);
	  
	  pql_add2array($domain_names, $additional);
	}
	// }}}
  }
  
  // {{{ Extra precaution: IDNA encode and uniqify all domain names 
  // IDNA encode the domain name(s)
  $count = count($domain_names);
  for($i=0; $i < $count; $i++)
	$domain_names[$i] = pql_maybe_idna_encode($domain_names[$i]);
  
  // Uniqify the list of domain names.
  $tmp = $domain_names; unset($domain_names);
  $domain_names = pql_uniq($tmp);
  // }}}
  
  return($domain_names);
}
// }}}

// {{{ pql_get_email_domains(host)
function pql_get_email_domains($host) {
  global $_pql;

  // {{{ Go through each base DN, retreive mail and mail aliases
  foreach($_SESSION["BASE_DN"] as $base) {
	$users = $_pql->search($base, pql_get_define("PQL_ATTR_MAIL").'=*', "SUBTREE");
	for($i=0; $i < count($users); $i++) {
	  $mails[] = $users[$i][pql_get_define("PQL_ATTR_MAIL")];
	  
	  if($users[$i][pql_get_define("PQL_ATTR_MAILALTERNATE")])
		pql_add2array($mails, $users[$i][pql_get_define("PQL_ATTR_MAILALTERNATE")]);
	}
  }
  // }}}
  
  // {{{ Extract only the domain name part from the email address(es)
  for($i=0; $i < count($mails); $i++) {
	$tmp = explode('@', $mails[$i]);
	$mails[$i] = pql_maybe_idna_encode($tmp[1]);
  }
  // }}}
  
  // {{{ Uniqify all domain names 
  $tmp = $mails; unset($mails);
  $mails = pql_uniq($tmp);
  // }}}
  
  return($mails);
}
// }}}

// {{{ pql_get_automount_maps(computerdn)
// Retreive ALL automount maps (recursivly) for a specific host in a specific domain
function pql_get_automount_maps($computerdn) {
  global $_pql;

  // {{{ Retreive the base mount points
  // Find the auto.master object
  $filter  = '(&';
  $filter .=   '(|';
  $filter .=     '('.pql_get_define("PQL_ATTR_OBJECTCLASS").'=autoMountMap)';
  $filter .=     '('.pql_get_define("PQL_ATTR_OBJECTCLASS").'=organizationalUnit)';
  $filter .=   ')';
  $filter .=   '(|';
  $filter .=     '('.pql_get_define("PQL_ATTR_CN").'=auto.master)';
  $filter .=     '('.pql_get_define("PQL_ATTR_OU").'=auto.master)';
  $filter .=   ')';
  $filter .= ')';

  $auto_master = $_pql->search($computerdn, $filter);
  $auto_master = $auto_master["dn"];

  if(isset($auto_master))
	// Get automount maps below auto.master
	$maps = pql_get_automount_map($auto_master);
  else
	// Get automount maps below computer DN
	$maps = pql_get_automount_map($computerdn);
  // }}}

  if(empty($maps))
	return NULL;
  else
	return($maps);
}
// }}}

// {{{ pql_get_automount_map(dn)
// Retreive automount maps below a specific mountpoint
function pql_get_automount_map($dn) {
  global $_pql, $_SESSION;

  // Retreive all automountMap(s) directly below the DN
  $filter = '(&(objectClass=automount)(|('.pql_get_define("PQL_ATTR_CN").'=*)('.pql_get_define("PQL_ATTR_OU").'=*)))';
  $maps = $_pql->search($dn, $filter, 'ONELEVEL');

  if(file_exists($_SESSION["path"]."/.DEBUG_AUTOMOUNT")) {
	echo "pql_get_automount_map(): Maps for DN '<b>$dn</b>': ";
	if($maps) 
	  printr($maps);
	else
	  echo "<i>empty</i><br>";
  }

  // Make sure the array is a _numbered_ array...
  if(is_array($maps) and !@$maps[0]) {
	$tmp = $maps;
	unset($maps);
	$maps[] = $tmp;
  }

  // For each of these base mount points, retreive all mount information
  if(is_array($maps)) {
	for($i=0; $maps[$i]; $i++) {
	  $cn = $maps[$i][pql_get_define("PQL_ATTR_CN")];
	  unset($maps[$i]["objectclass"]);

	  if(preg_match('/ldap/i', $maps[$i][pql_get_define("PQL_ATTR_AUTOMOUNT_INFO")])) {
		// Extract any links to another LDAP automount map
		// [automountinformation] => -fstype=autofs ldap:ou=auto.mnt.cdrom,ou=Automounts,o=Main Branch,c=SE
		$dn = preg_replace('/.*:/', '', $maps[$i][pql_get_define("PQL_ATTR_AUTOMOUNT_INFO")]);

		// Call ourselfs recursivly to get automount maps below this mountpoint.
		$AUTOMASTER[$cn] = pql_get_automount_map($dn);
	  } else {
		if($maps[$i][pql_get_define("PQL_ATTR_DESCRIPTION")])
		  $AUTOMASTER[$cn][pql_get_define("PQL_ATTR_DESCRIPTION")] = $maps[$i][pql_get_define("PQL_ATTR_DESCRIPTION")];
		else
		  $AUTOMASTER[$cn][pql_get_define("PQL_ATTR_DESCRIPTION")] = '';
		
		if($maps[$i][pql_get_define("PQL_ATTR_AUTOMOUNT_INFO")])
		  $AUTOMASTER[$cn][pql_get_define("PQL_ATTR_AUTOMOUNT_INFO")] = $maps[$i][pql_get_define("PQL_ATTR_AUTOMOUNT_INFO")];
		else
		  $AUTOMASTER[$cn][pql_get_define("PQL_ATTR_AUTOMOUNT_INFO")] = '';

		// Remember the DN (normalized) so we can easily reference it
		$AUTOMASTER[$cn]["dn"] = pql_format_normalize_dn($maps[$i]["dn"]);
	  }
	}
  }

  return($AUTOMASTER);
}
// }}}

// {{{ pql_search_forwarders(user)
// Search all accounts with forwarders to $user@$domain
function pql_search_forwarders($user) {
  global $_pql;
  $return = '';
  
  // get all email addresses of a user
  $email   = $_pql->get_attribute($user, pql_get_define("PQL_ATTR_MAIL"));
  $aliases = $_pql->get_attribute($user, pql_get_define("PQL_ATTR_MAILALTERNATE"));
  
  $addresses[] = $email;
  if(is_array($aliases))
	$addresses = array_merge($addresses, $aliases);
  
  // create filter
  $filter = "(|";
  foreach($addresses as $add)
	$filter .= "(" . pql_get_define("PQL_ATTR_FORWARDS") . "=" . $add . ")";
  $filter .= ")";
  
  // Go through each base DN in the database, looking
  // for forwarders to this user
  foreach($_SESSION["BASE_DN"] as $dn) {
	$ref = pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $dn);
	
	// Get all users that refere/forwards to this ($dn) object.
	$results = $_pql->get_dn($dn, $filter);
	for($i=0; $i < count($results); $i++) {
	  // Retreive this object.
	  $object = $_pql->search($results[$i], pql_get_define("PQL_ATTR_OBJECTCLASS").'=*', 'BASE');
	  
	  $return[] = array("reference" => $object[0]['dn'],
						$ref		=> $object[0][$ref],
						"email"     => $object[0][pql_get_define("PQL_ATTR_MAIL")]);
	  
	} // end foreach results
  } // end foreach basedn
  
  return $return;
}
// }}}

// {{{ pql_email_exists(email)
// Check if any mail or mailalternateaddress record with this email exists in the ldap tree
function pql_email_exists($email) {
  global $_pql;

  $filter = "(|(" . pql_get_define("PQL_ATTR_MAIL") . "=" . $email . ")(" . pql_get_define("PQL_ATTR_MAILALTERNATE") . "=" . $email . "))";
  foreach($_SESSION["BASE_DN"] as $dn) {
	$results = $_pql->get_dn($dn, $filter);
	if(is_array($results))
	  return true;
  }
  
  return false;
}
// }}}

// {{{ pql_validate_administrator(dn, attrib, admin)
function pql_validate_administrator($dn, $attrib, $admin) {
  global $_pql;

  $result = $_pql->get_attribute($dn, $attrib);
  if(!$result)
	return(false);
  
  if(is_array($result)) {
	foreach($result as $res) {
	  $res = pql_format_normalize_dn($res);
	  if($admin == $res)
		return(true);
	}
  } elseif(pql_format_normalize_dn($result) == $admin)
	return(true);
}
// }}}

// {{{ pql_validate_object(dn, object, type)
// Verify that the object have what it needs to be added:
// 1. All object classes choosen have their MUST attributes defined.
// 2. All attributes have a object class specified (TODO).
// 3. The ACI attribute is defined in the object
// Returns a new, improved/correct object.
// NOTE: $dn and $type is only used/needed for the ACI's...
function pql_validate_object($dn, $object, $type) {
  global $_pql;
  $linkid = $_pql->ldap_linkid;

  // {{{ Encode values that needs to encoded and extract the object classes
  foreach($object as $key => $value) {
	// Check if we have a MUST in the objectclasses choosen
	// for creation for this attribute...
	if(is_array($value)) {
	  foreach($value as $val) {
		if(preg_match('/'.pql_get_define("PQL_ATTR_OBJECTCLASS").'/i', $key))
		  // Remember the object class for later
		  $ocs[] = $val;
		else
		  $entry[$key][] = pql_maybe_encode(trim($val), $key, $linkid);
	  }
	} else {
	  if(preg_match('/'.pql_get_define("PQL_ATTR_OBJECTCLASS").'/i', $key))
		// Remember the object class for later
		$ocs[] = $value;
	  else
		$entry[$key] = pql_maybe_encode(trim($value), $key, $linkid);
	}
  }
  // }}}
  
  // Retreive all objectclasses the LDAP server knows about
  $ldap = pql_get_subschema("objectclasses");

  // {{{ Go through the WANTED objectclasses, checking for a MUST attribute we don't have
  if(is_array($ocs)) {
	foreach($ocs as $oc) {
	  $oc = lc($oc);
	  
	  if(empty($ldap[$oc]['MUST']['count']))
		// An objectclass with no MUST, add this directly
		$entry[pql_get_define("PQL_ATTR_OBJECTCLASS")][] = $oc;
	  else {
		// This objectclass have one or more MUST attributes
		// Do we have any of those MUST attributes defined?
		$MUST = $ldap[$oc]['MUST'];
		
		foreach($object as $attrib => $value) {
		  $attrib = lc($attrib);
		  
		  // Go through the MUST attributes for this objctclass,
		  // looking for an attribute we have NOT defined
		  for($i=0; $i < $MUST["count"]; $i++) {
			$must = lc($MUST[$i]);
			
			if(preg_match('/:/', $must)) {
			  // We have alias(es). Check each for a match!
			  $aliases = explode(':', $must);
			  for($j=0; $j < count($aliases); $j++) {
				if($attrib == $aliases[$j]) {
				  unset($MUST[$i]);
				  break;
				}
			  }
			} else {
			  if($attrib == $must)
				unset($MUST[$i]);
			}
		  }
		  
		  // Before we rearrange the array, remove the count
		  // entry (othervise it gets renamed to a number).
		  unset($MUST['count']);
		  
		  // Rearrange the array
		  $MUST = pql_uniq($MUST);
		  
		  // Readd the count entry
		  $MUST['count'] = count($MUST);
		}
		
		if(!$MUST['count'])
		  // This objectclass don't contain MUST attributes which
		  // isn't defined in the user object. Add this to the list
		  // of objectclasses to add
		  $entry[pql_get_define("PQL_ATTR_OBJECTCLASS")][] = $oc;
		else {
		  // Objectclass which have been removed from the list because
		  // there's an MUST attribute missing
		  echo "<b>Objectclass with missing attribute in MUST:</b> <u>$oc</u><br>";
		  echo "You're missing the following attributes: ";
		  for($i=0; $i < $MUST['count']; $i++) {
			if(!$MUST[$i+1] and ($i != 0))
			  echo "and ";
			
			echo "<i>".$MUST[$i]."</i>";
			
			if($MUST[$i+1])
			  echo ", ";
		  }
		  echo "<p>";
		}
	  }
	}
  }
  // }}}
  
  // {{{ Go through each attribute, verifying that we have an object class for this.
//  foreach($object as $attrib => $value) {
//	$attrib = lc($attrib);
//	
//	if(!preg_match('/'.pql_get_define("PQL_ATTR_OBJECTCLASS").'/i', $attrib)) {
//	  // This is a non-object class attribute
//
//	  // TODO: How do we check that this attribute have a object class specified in $entry!?
//	}
//  }
  // }}}
  
  // {{{ Add the OpenLDAPaci attribute (maybe)
  global $_pql;
  $acis = $_pql->get_attribute($dn, pql_get_define("PQL_ATTR_LDAPACI"), 1);
  if($_SESSION["ACI_SUPPORT_ENABLED"] and function_exists("user_generate_aci") and
     empty($entry[pql_get_define("PQL_ATTR_LDAPACI")]) and !is_array($acis))
	$entry[pql_get_define("PQL_ATTR_LDAPACI")] = user_generate_aci($dn, $type);
  // }}}
  
  return($entry);
}
// }}}

// {{{ pql_get_subschemas()
function pql_get_subschemas() {
  $attribs = array("ldapsyntaxes", "matchingrules", "attributetypes", "objectclasses");
  foreach($attribs as $attrib) {
	$result = pql_get_subschema($attrib);
	if($result)
	  $entry[$attrib] = $result;
  }
  
  // Return 4 arrays with:
  //	ldapsyntaxes
  //	matchingrules
  //	attributetypes
  //	objectclasses
  return $entry;
}
// }}}

// {{{ pql_get_subschema(attrib)
function pql_get_subschema($attrib) {
  global $_pql;

  $attrib = lc($attrib);
  
  // Get the DN for the subSchema
  // CMD: /usr/bin/ldapsearch -x -LLL -h localhost -s base -b '' 'objectClass=*' subschemaSubentry
  //
  // TODO: Reuse the $this->_find_base_option() function here!
  $entry = $_pql->get_attribute(NULL, 'subschemaSubentry', 1);
  if($entry) {
	// Get the (operational) attribute subSchemaAttributes from the subSchema
	// CMD: /usr/bin/ldapsearch -x -LLL -h localhost -s base -b 'cn=Subschema' 'objectClass=subSchema' $attrib
	$entry = $_pql->search($entry, pql_get_define("PQL_ATTR_OBJECTCLASS").'=subSchema', 'BASE', 0, $entry);
	if(!empty($entry)) {
	  // Catch a flat object - i.e. only ONE domain
	  if(empty($entry[0])) {
		$tmp = $entry; unset($entry);
		$entry[0] = $tmp; unset($tmp);
	  }

	  // {{{ Examples (each objectClasses attribute on ONE line!):
	  // * ldapSyntaxes
	  //	ldapSyntaxes: ( 1.3.6.1.4.1.1466.115.121.1.49 DESC 'Supported Algorithm' X-BINARY-TRANSFER-REQUIRED 'TRUE' X-NOT-HUMAN-READABLE 'TRUE' )
	  //	ldapSyntaxes: ( 1.3.6.1.4.1.1466.115.121.1.45 DESC 'SubtreeSpecification' )
	  //
	  //	-> The X-BINARY-TRANSFER-REQUIRED and X-NOT-HUMAN-READABLE seems to
	  //	   be optional...
	  //
	  // * matchingRules
	  //	matchingRules: ( 2.5.13.5 NAME 'caseExactMatch' SYNTAX 1.3.6.1.4.1.1466.115.121.1.15 )
	  //
	  //	-> Very simple and uniform...
	  //
	  // * attributeTypes
	  //	attributeTypes: ( 2.5.4.35 NAME 'userPassword' DESC 'RFC2256/2307: password of user' 
	  //		EQUALITY octetStringMatch SYNTAX 1.3.6.1.4.1.1466.115.121.1.40{128} )
	  //	attributeTypes: ( 2.5.4.3 NAME ( 'cn' 'commonName' ) DESC 'RFC2256: common name(s) for which the entity is known by' 
	  //		SUP name )
	  //	attributeTypes: ( 1.3.6.1.4.1.4203.666.1.12 NAME 'queryid' DESC 'list of queries the entry belongs to' 
	  //		EQUALITY octetStringMatch SYNTAX 1.3.6.1.4.1.1466.115.121.1.40{64} NO-USER-MODIFICATION 
	  //		USAGE directoryOperation )
	  //
	  //	-> Only NAME and DESC seems to be required, all the rest is optional.
	  //
	  // * objectClasses
	  //	objectClasses: ( 2.5.6.6 NAME 'person' DESC 'RFC2256: a person' 
	  //		SUP top STRUCTURAL MUST ( sn $ cn ) MAY ( userPassword $ 
	  //		telephoneNumber $ seeAlso $ description ) )
	  //	objectClasses: ( 1.3.6.1.4.1.7914.1.2.2.1 NAME 'qmailUser' DESC 
	  //		'QMail-LDAP User' SUP top AUXILIARY MUST mail MAY ( uid $ 
	  //		mailMessageStore $ homeDirectory $ userPassword $ mailAlternateAddress $ 
	  //		qmailUID $ qmailGID $ mailHost $ mailForwardingAddress $ deliveryProgramPath $ 
	  //		qmailDotMode $ deliveryMode $ mailReplyText $ accountStatus $ qmailAccountPurge $ 
	  //		mailQuotaSize $ mailQuotaCount $ mailSizeMax ) )
	  //
	  //	-> If there's only one MUST/MAY then there's no parentheses, but
	  //	   if there's more than one, then there's parentheses around the
	  //	   attributes, just like a schema definition.
	  //
	  // * Problem (on Fedora Directory Server)
	  //	objectclasses: ( 2.5.6.6 NAME 'person' DESC 'Standard LDAP objectclass' 
	  //		SUP top STRUCTURAL MUST ( sn $ cn ) MAY ( description $ seeAlso $ 
	  //		telephoneNumber $ userPassword ) X-ORIGIN 'RFC 2256' )
	  // }}}

	  for($i=0; $i < count($entry[0][$attrib]); $i++) {
		// {{{ Retreive NAME
		$words = explode("NAME", $entry[0][$attrib][$i]);
		if(!empty($words[1])) {
		  if(preg_match("/^ \(/", $words[1])) {
			$words = explode("\)", $words[1]);
			$words = ereg_replace("\'", "", $words[0]);
			$words = explode(" ", $words);
			$name  = lc($words[2]);
			
			$VALUE[$name]["NAME"] = $words[2];
			for($j=3; $j < count($words); $j++) {
			  $VALUE["ALIAS"][$name][] = $words[$j];
			  $VALUE["ALIAS"][$words[$j]][] = $name;
			  
			  $VALUE[$name]["ALIAS"][] = $words[$j];
			}
		  } else {
			$words = explode("\' ", $words[1]);
			$words = explode(" \'", $words[0]);
			$name  = lc($words[1]);
			
			$VALUE[$name]["NAME"] = $words[1];
		  }
		} elseif($attrib == 'ldapsyntaxes') {
		  $words = explode(" ", $entry[0][$attrib][$i]);
		  $name  = lc($words[1]);
		}
		// }}}

		// {{{ Get the OID number
		$words = explode(" ", $entry[0][$attrib][$i]);
		$VALUE[$name]["OID"] = $words[1];
		// }}}
		
		// {{{ Retreive DESC
		$words = explode("DESC", $entry[0][$attrib][$i]);
		if(! empty($words[1])) {
		  $words = explode("\' ", $words[1]);
		  $words = explode(" \'", $words[0]);
		  
		  $VALUE[$name]["DESC"] = $words[1];
		}
		// }}}
		
		// {{{ Retreive MAY's
		$words = explode("MAY", $entry[0][$attrib][$i]);
		if(! empty($words[1])) {
		  if(preg_match("/\(/", $words[1])) {
			$words = explode("\(", $words[1]);
			$words = explode("\)", $words[1]);
		  } else {
			$words = explode("\)", $words[1]);
		  }
		  
		  // Remove spaces
		  $words = ereg_replace(" ", "", $words[0]);
		  $words = explode('\$', $words);
		  
		  $j = 0;
		  foreach($words as $may) {
			$VALUE[$name]["MAY"][] = $may;
			$j++;
		  }
		  $VALUE[$name]["MAY"]["count"] = $j;
		}
		// }}}
		
		// {{{ Retreive MUST's
		$words = explode("MUST", $entry[0][$attrib][$i]);
		if(! empty($words[1])) {
		  if(preg_match("/^ \(/", $words[1])) {
			$words = explode("\(", $words[1]);
			$words = explode("\)", $words[1]);
			$word  = $words[0];
		  } else {
			$words = explode(" ", $words[1]);
			$word  = $words[1];
		  }
		  
		  // Remove spaces
		  $words = ereg_replace(" ", "", $word);
		  $words = explode('\$', $words);
		  
		  $j = 0;
		  foreach($words as $must) {
			$VALUE[$name]["MUST"][] = $must;
			$j++;
		  }
		  $VALUE[$name]["MUST"]["count"] = $j;
		}
		// }}}
		
		// {{{ Retreive EQUALITY
		$words = explode("EQUALITY", $entry[0][$attrib][$i]);
		if(! empty($words[1])) {
		  $words = explode(" ", $words[1]);
		  $VALUE[$name]["EQUALITY"] = $words[1];
		}
		// }}}
		
		// {{{ Retreive SYNTAX
		$words = explode("SYNTAX", $entry[0][$attrib][$i]);
		if(! empty($words[1])) {
		  $words = explode(" ", $words[1]);
		  $VALUE[$name]["SYNTAX"] = $words[1];
		}
		// }}}

		// {{{ Retreive USAGE
		$words = explode("USAGE", $entry[0][$attrib][$i]);
		if(! empty($words[1])) {
		  $words = explode(" ", $words[1]);
		  $VALUE[$name]["USAGE"] = $words[1];
		}
		// }}}
		
		// {{{ Retreive SUP
		if(preg_match("/SUP/i", $entry[0][$attrib][$i])) {
		  $words = explode("SUP", $entry[0][$attrib][$i]);
		  if(! empty($words[1])) {
			if(preg_match("/^ \(/", $words[1])) {
			  $words = explode("\(", $words[1]);
			  $words = explode("\)", $words[1]);
			  
			  // Remove spaces
			  $words = ereg_replace(" ", "", $words[0]);
			  $words = explode('\$', $words);
			  
			  for($j=0; $j < count($words); $j++)
				$VALUE[$name]["SUP"][] = $words[$j];
			} else {
			  $words = explode(" ",  $words[1]);
			  
			  // Remove spaces
			  $VALUE[$name]["SUP"] = ereg_replace(" ", "", $words[1]);
			}
		  }
		}
		// }}}
		
		// {{{ Retreive X-ORIGIN (Fedora Directory Server)
		$words = explode("X-ORIGIN", $entry[0][$attrib][$i]);
		if(! empty($words[1])) {
		  $words = explode("\' ", $words[1]);
		  $words = explode(" \'", $words[0]);
		  
		  $VALUE[$name]["X-ORIGIN"] = $words[1];
		}
		// }}}

		// {{{ Retreive SINGLE-VALUE
		if(preg_match("/SINGLE-VALUE/i", $entry[0][$attrib][$i]))
		  $VALUE[$name]["SINGLE-VALUE"] = 'TRUE';
		// }}}
		
		// {{{ Retreive NO-USER-MODIFICATION
		if(preg_match("/NO-USER-MODIFICATION/i", $entry[0][$attrib][$i]))
		  $VALUE[$name]["NO-USER-MODIFICATION"] = 'TRUE';
		// }}}
		
		// {{{ Retreive X-BINARY-TRANSFER-REQUIRED
		if(preg_match("/X-BINARY-TRANSFER-REQUIRED/i", $entry[0][$attrib][$i]))
		  $VALUE[$name]["X-BINARY-TRANSFER-REQUIRED"] = 'TRUE';
		// }}}
		
		// {{{ Retreive X-NOT-HUMAN-READABLE
		if(preg_match("/X-NOT-HUMAN-READABLE/i", $entry[0][$attrib][$i]))
		  $VALUE[$name]["X-NOT-HUMAN-READABLE"] = 'TRUE';
		// }}}
	  }
	}
	
	/* If there's no anonymous read access to the 'cn=Subschema',
	 * phpQLAdmin will fail here...  */
	if(!empty($VALUE)) {
	  if(is_array($VALUE))
		ksort($VALUE);
	
	  // TODO: Document what/how it works - IT'S UGGLY!!!
	  if($attrib == 'objectclasses') {
		// There might be aliases, so we must retreive all attribute types...
		// All attribute(s) that have aliases is in $ATTRS["ALIAS"].
		$ATTRS = pql_get_subschema('attributeTypes');
		
		if(!empty($VALUE["ALIAS"])) {
		  // Save this so it doesn't interfere
		  $OC_ALIAS = $VALUE["ALIAS"];
		  unset($VALUE["ALIAS"]);
		}
		
		// Go through each object class
		foreach($VALUE as $value_attrib => $value_data) {
		  if(!empty($value_data["MUST"])) {
			// Go through each MUST attribute(s)
			for($j=0; $j < $value_data["MUST"]["count"]; $j++) {
			  if(!empty($ATTRS["ALIAS"])) {
				// Go through each attribute alias(es)
				foreach($ATTRS["ALIAS"] as $attrs_attrib => $attrs_data) {
				  if(!empty($value_data["MAY"][$j]) and
					 (lc($value_data["MAY"][$j]) == lc($attrs_attrib)))
					{
					  foreach($attrs_data as $alias) {
						if(!empty($alias))
						  $VALUE[$value_attrib]["MAY"][$j] .= ":$alias";
					  }
					}
				  
				  if(!empty($value_data["MUST"][$j]) and
					 (lc($value_data["MUST"][$j]) == lc($attrs_attrib)))
					{
					  foreach($attrs_data as $alias) {
						if(!empty($alias))
						  $VALUE[$value_attrib]["MUST"][$j] .= ":$alias";
					  }
					}
				}
			  }
			}
		  }
		}
		
		if(!empty($OC_ALIAS)) {
		  // Add the object class alias(es) back
		  $VALUE["ALIAS"] = $OC_ALIAS; 
		}
	  }
	}

	if(empty($VALUE))
	  return false;
	else
	  return $VALUE;
  } else
	return false;
}
// }}}

// {{{ pql_get_rootdn(dn)
function pql_get_rootdn($dn, $function = '') {
  global $_pql, $_SESSION;

  $tracker = 'Please report this at the <a href="http://bugs.bayour.com/" target="_new">bugtracker</a>.<br>';
  
  if($function)
	$function = " (from $function)";
  
  if($dn == '')
	die("This is weird. We're called$function with an empty DN! $tracker");
  else
	$dn = urldecode($dn);
  
  if(is_array($_SESSION["BASE_DN"])) {
	$counts = 0;
	
	// Get all possible matches. If we're looking for 'uid=turbo,ou=People,o=Fredriksson,c=SE',
	// there is at least FOUR possibilities (one for each and every branch - they can all be
	// separate root DN's!
	foreach($_SESSION["BASE_DN"] as $base) {
	  if(preg_match("/$base\$/i", $dn))  {
		$counts++;
		$DNs[] = $base;
	  }
	}
	
	if(($counts < 1) and @$_SESSION["MONITOR_BACKEND_ENABLED"]) {
	  // We have ONE more shot in finding the root DN. Look in the
	  // 'cn=Databases,cn=Monitor:namingContexts' attribute(s).
	  require($_SESSION["path"]."/include/pql_status.inc");
	  $tmp = pql_get_status($_pql->ldap_linkid, "cn=Databases,".$_pql->ldap_monitor, 'namingContexts');
	  if(is_array($tmp)) {
		// We got more than one namingContexts (i.e. an array). Go through each value...
		foreach($tmp as $base) {
		  if(preg_match("/$base\$/i", $dn))  {
			$counts++;
			$DNs[] = $base;
		  }
		}
	  } elseif(preg_match("/$tmp/i", $dn)) {
		// We've only got one namingContexts (i.e. not an array).
		$counts++;
		$DNs[] = $tmp;
	  }
	}
	
	if($counts >= 2) {
	  // We have more than one hit (there's at least TWO partitions)!!
	  // -> Check each DN, looking for the DN with the HIGHEST amount of commas.
	  //    That must be the DN that's closest to the DN we're looking for.
	  foreach($DNs as $base)  {
		$dn_parts = explode(',', $base);
		$count = count($dn_parts); // Get amount of DN parts.
		
		// Remember this DN. It's above the DN we're looking for.
		$DN_REF[$base] = ($count) ? $count : 0;
	  }
	  
	  // Again we have more than one. Which of these have the highest
	  // amount of commas!?
	  if($DN_REF) {
		asort($DN_REF); // Sort the DN's left. The LAST one have the most commas!
		
		// Just go through them one by one to get the last one...
		foreach($DN_REF as $base => $count)
		  $found = $base;
		
		// Return this DN. It MUST be the correct one!!
		return(urlencode($found));
	  }
	  
	  // We just shouldn't end up here! "It can't happen" - famous last words :)
	  die("You've ended up in a place where I didn't expect you to get... How exactly did you get here!? $tracker");
	} elseif($counts < 1) {
	  // BUG! We don't have a hit!
	  echo "This is weird. We couldn't find the root dn for some reason. The DN we're trying to find a root DN for is: ";
	  echo "'$dn'.<p>$tracker<p>";
	  echo "Note that you <u><b>MUST</b></u> include exact details on how you got this error.";
	  echo "Every single mouse click etc - an URL won't suffice!";
	  die();
	} else
	  $dn = $DNs[0];
  } else {
	// We don't have any base dn's. Maybe because
	// we're included from a file which doesn't
	// have a connection to the LDAP server.
	//
	// Find the root dn the 'old fasioned' (ie,
	// broken!) way.
	$rootdn = explode(',', $dn);
	
	if($rootdn[1]) {
	  unset($rootdn[0]);
	  $dn = implode(",", $rootdn);
	} else
	  $dn = $rootdn[count($rootdn)-1];
  }
  
  return(urlencode($dn));
}
// }}}

// {{{ pql_setup_branch_objectclasses(filter, dn, linkid)
// Setup object class entry for a domain/branch with all
// necessary objectclasses
function pql_setup_branch_objectclasses($filter = 0, $dn, $linkid = '') {
  if($dn and $linkid) {
	$dn = pql_format_normalize_dn($dn);

	// Retreive existing objectClasses from the object
	$object = ldap_explode_dn($dn, 0);
	$sr     = @ldap_read($linkid, $dn, $object[0], array(pql_get_define("PQL_ATTR_OBJECTCLASS")));
	$ocs    = @ldap_get_entries($linkid, $sr) or pql_format_error(1);
	for($j=0; $j < $ocs[0][pql_get_define("PQL_ATTR_OBJECTCLASS")]["count"]; $j++) {
	  if(preg_match('/phpQLAdminBranch/i', $ocs[0][pql_get_define("PQL_ATTR_OBJECTCLASS")][$j]))
		$got_phpqladmin_objectclass_branch = 1;
	  
	  $objectclass[] = $ocs[0][pql_get_define("PQL_ATTR_OBJECTCLASS")][$j];
	}
  } else {
	// No previous DN, we're probably creating a new
	
	// ObjectClasses for this root DN
	if($dn) {
	  // What's the Root DN (namingContexts) for this domain
	  $rootdn = pql_get_rootdn($dn, 'pql_setup_branch_objectclasses'); $rootdn = urldecode($rootdn);
	  
	  $ocs = pql_get_define("PQL_CONF_OBJECTCLASS_DOMAIN", $rootdn);
	} elseif($_SESSION["BASE_DN"][0])
	  $ocs = pql_get_define("PQL_CONF_OBJECTCLASS_DOMAIN", $_SESSION["BASE_DN"][0]);
	else
	  $ocs = '';

	if($filter)
	  $entry = '';
	
	$ocs = pql_split_oldvalues($ocs);
	foreach($ocs as $oc) {
	  if($filter) {
		if(!preg_match('/dcOrganizationNameForm/i', $oc))
		  $entry .= "(objectClass=$oc)";
	  } else {
		if(preg_match('/phpQLAdminBranch/i', $oc))
		  $got_phpqladmin_objectclass_branch = 1;
		
		$entry[] = $oc;
	  }
	}
  }
  
  if($filter and $entry)
	return($entry);
  
  if($dn and $linkid)
	return($objectclass);
  
  if(!$got_phpqladmin_objectclass_branch and !$filter) {
	$entry[] = 'phpQLAdminBranch';
	for($i=0; $i < count($objectclass); $i++)
	  $entry[] = $objectclass[$i];
  }
  
  if(!count($entry))
	// We have already setup the objectclasses, return NULL
	return;
  else
	// Return the objectClasses
	return($entry);
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
