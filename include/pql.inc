<?php
// ----------------------------
// pql.inc
// phpQLAdmin Application Programming Interface (API)
//
// $Id: pql.inc,v 2.200.2.2 2005-02-19 08:27:39 turbo Exp $
//

// ----------------------------
// Get release version
$fp = fopen($_SESSION["path"]."/.version", "r");
$_SESSION["VERSION"] = fgets($fp, 50);
$_SESSION["VERSION"] = rtrim($_SESSION["VERSION"], "\n");
fclose($fp);

// ----------------------------
// turn on output-buffering by default
ob_start();

// ----------------------------
// API classes
// ----------------------------

// {{{ PQL - main class (connection handling)
class pql {
	var $ldap_linkid = 0;					// ldap connection identifier
	var $ldap_error  = 0;					// error message incase of failiure
	var $ldap_basedn = 0;					// array of base dns for future searches etc
	
	// {{{ string _find_base_option(attribute)

	function _find_base_option($attrib) {
	    $attrib = lc($attrib);

	    $sr = @ldap_read($this->ldap_linkid, NULL, '(objectClass=*)', array($attrib));
	    if(! $sr) die("Can't find base dn - ".ldap_error($this->ldap_linkid));
	    $entry = ldap_get_entries($this->ldap_linkid, $sr);
//printr($entry); die();

	    // Build an array of base dn's. It's possible to have multiple in a database
	    for($i=0; $i < $entry["count"]; $i++) {
			if(is_array(@$entry[$i][$attrib])) {
				for($j=0; $j < $entry[$i][$attrib]["count"]; $j++ )
				  $val[] = urlencode(pql_maybe_encode($entry[$i][$attrib][$j], $attrib, $this->ldap_linkid));
			} elseif($entry[$i]["dn"]) {
				$val[] = urlencode(pql_maybe_encode($entry[$i]["dn"], $attrib, $this->ldap_linkid));
			} else
			  $val[] = urlencode(pql_maybe_encode($entry[$i][$attrib][0], $attrib, $this->ldap_linkid));

			$success = 1;
	    }
	    
	    // If we got at least one DN, return the array. Else return false
	    if($success) {
			$new = NULL;

			// Remove any spaces after the RDN separator
			foreach($val as $dn) {
				while(eregi("%2C\+", $dn)) {
					// Replace ', ' with ','
					$new  = eregi_replace("%2C\+", "%2C", $dn);
				}
			}

			if(is_array($new))
			  return $new;
			else
			  return $val;
		} else
		  return false;
	}

	// }}}

	// {{{ string _find_basedn(void)
	function _find_basedn() {
	    // CMD: /usr/bin/ldapsearch -x -LLL -h localhost -s base -b '' objectclass=* namingContexts
	    return $this->_find_base_option("namingContexts");
	}
	// }}}

	// {{{ bool pql(hostname, binddn, bindpw, noconnect, dodie)
	function pql($host, $binddn = "", $bindpw = "", $no_connect = false, $do_die = 1) {
		// constructor, connect to ldap host
		if($no_connect == true)
		  return false;

		// Open a connection to the host
		$this->connect($host) or die("<b>could not connect to ldap server</b>");

		// Setup connection (set options etc)
		$this->set_options($this->ldap_linkid);

		// It's possible to be called without binddn, but with a password.
		// Can't use the password in that case...
		if(!$binddn && $bindpw)
		  unset($bindpw);

		// Bind to the host
		if(!$this->bind($binddn, $bindpw)) {
			$this->ldap_error = "<b>Could not bind to ldap server</b>: ";
			$this->ldap_error = $this->ldap_error . ldap_error($this->ldap_linkid) . "<p>" .
			  "Host: $host, Port: $port<br>" .
			  "Please <a href=\"index.php?logout=1\" target=\"_top\">relogin</a>.<p>" .
			  "binddn='$binddn', bindpw='$bindpw'.";

			if($do_die)
			  die($this->ldap_error);

			return false;
		}

		// Find the base DN(s)
		$this->ldap_basedn = $this->_find_basedn();

		return true;
	}
	// }}}

	// {{{ bool set_options() {
	function set_options() {
		// LDAP_OPT_REFERRALS
		//	Automatically follow referrals returned by LDAP server?
		//	LDAP_OPT_OFF			 0
		//	LDAP_OPT_ON				!0 (default)
		//
		// NOTE: Enabling this if the LDAP server is an AD is a bad idea
		ldap_set_option($this->ldap_linkid, LDAP_OPT_REFERRALS,	0);

		// LDAP_OPT_DEREF
		//	Deterime how aliases are handled during search.
		//	LDAP_DEREF_NEVER		 0 (default)
		//	LDAP_DEREF_SEARCHING	 1
		//	LDAP_DEREF_FINDING		 2
		//	LDAP_DEREF_ALWAYS		 3
		ldap_set_option($this->ldap_linkid, LDAP_OPT_DEREF,     0);

		if($this->ldap_linkid) {
			// Start with a LDAPv3 bind
			if(! @ldap_set_option($this->ldap_linkid, LDAP_OPT_PROTOCOL_VERSION, 3)) {
				// Didn't work. Try a LDAPv2 bind...
				echo "Don't support v3 bind, trying to do a v2 bind.<br>";
				
				if(! @ldap_set_option($this->ldap_linkid, LDAP_OPT_PROTOCOL_VERSION, 2))
				  echo "Neither protocol LDAPv3 nor LDAPv2 worked, strange!<p>LDAP Error: ".ldap_error($this->ldap_linkid);
				
				return false;
			}

			// We're using LDAPv3, so try a TLS initialization.
			// Only availible in PHP v4.2.0, hence tripple check
			// that that function is availible before trying to
			// use it.
			if(function_exists("ldap_start_tls") and pql_get_define("PQL_CONF_USE_TLS")) {
				if(! ldap_start_tls($this->ldap_linkid))
				  echo "TLS initialization failed: ".ldap_error($this->ldap_linkid);
			}

			return true;
		}
	}

	// }}}

	// {{{ bool connect(hostname)
	function connect($host) {
		// Check to see if we have a port defined in the host.
		if(ereg(";", $host)) {
			$ldap = split(";", $host);
			
			$host = $ldap[0];
			$port = $ldap[1];
		} else {
			$host = $host;
			$port = 389;
		}

		// connect to ldap host and bind to root_dn
		$this->ldap_linkid = @ldap_connect($host, $port);

		return true;
	}
	// }}}

	// {{{ bool bind(binddn, bindpw)
	function bind($binddn = "", $bindpw = "") {
		// If we haven't set the DN/PW -> do a anonymous bind
		if($this->ldap_linkid and @ldap_bind($this->ldap_linkid, $binddn, $bindpw)) {
			return true;
		}
		
		return false;
	}
	// }}}

	// {{{ int close(void)
	function close(){
		// close connection to ldap host
		@ldap_unbind($this->ldap_linkid);
	}
	// }}}
	
	// {{{ bool connected(void)
	function connected() {
		if($this->ldap_linkid != false){
  			return true;
		}
	}
	// }}}
}

// }}}

// ------------------------------------------------
// API functions - support
// ------------------------------------------------

// {{{ pql_user_get_quota(ldap_linkid, user)
// Get formated quota of a user
function pql_user_get_quota($ldap_linkid, $user) {
	if($_SESSION["NEW_STYLE_QUOTA"]) {
		$quota_size  = pql_get_attribute($ldap_linkid, $user, pql_get_define("PQL_ATTR_QUOTA_SIZE"));
		$quota_count = pql_get_attribute($ldap_linkid, $user, pql_get_define("PQL_ATTR_QUOTA_COUNT"));
		
		if($quota_size and $quota_count)
		  $quota = array($quota_size[0]."S,".$quota_count[0]."C");
		else
		  // Just incase we haven't the new style quota in the DB, get the old type
		  $quota = pql_get_attribute($ldap_linkid, $user, pql_get_define("PQL_ATTR_QUOTA_VALUE"));
	} else
	  // We're not using new style quota, get the old type...
	  $quota = pql_get_attribute($ldap_linkid, $user, pql_get_define("PQL_ATTR_QUOTA_VALUE"));

	if(!is_array($quota))
	  return false;
	else
	  return pql_parse_quota($quota[0]);
}

// }}}

// {{{ pql_user_get_letter(linkid, domain, letra)
// Based in PQL_CONF_GET_USER to search by the cn first letter
function pql_user_get_letter($linkid, $domain, $letra) {
	$filter = "(".pql_get_define("PQL_ATTR_UID")."=".$letra."*)";
	$user = pql_search($linkid, $domain, $filter);
    return $user;
}

// }}}

// {{{ pql_user_get_number(ldap_linkid, domain)
// Based in PQL_CONF_GET_USER to search by an initial number at cn attribute
function pql_user_get_number($linkid, $domain) {
	// What's the Root DN (namingContexts) for this domain
	$rootdn = pql_get_rootdn($dn, 'pql_user_get_number'); $rootdn = urldecode($rootdn);

    for($numero=0; $numero <= 9; $numero++) {
		$filter = "(".pql_get_define("PQL_ATTR_UID")."=".$numero."*)";
		$tmp = pql_search($linkid, $domain, $filter);
		
		if($user)
		  $user = $user + $tmp;
    }
    
    return $user;
}

// }}}

// This functions isn't used any where - yet?
// {{{ pql_user_modify_addressbook(ldap_linkid, user, attribs)
function pql_user_modify_addressbook($ldap_linkid, $user, $attribs) {
	// What's the Root DN (namingContexts) of this user
	$rootdn = pql_get_rootdn($user, 'pql_user_modify_addressbook'); $rootdn = urldecode($rootdn);

    // check if addressbook is disabled
    if(!pql_get_define("PQL_CONF_USE_ADDRESSBOOK", $rootdn))
	  return false;
    
    // object-class dependencies for addressbook entries
    //
    // ------------------------------------------------------------------------------------
    // | OBJECTCLASS          | Must-Attr. (OpenLDAP 1.2.x) | Must-Attr. (OpenLDAP 2.0.x) |
    // ------------------------------------------------------------------------------------
    // | InetOrgPerson        | none                        | none                        |
    // ------------------------------------------------------------------------------------
    // | organizationalPerson | sn, cn                      | none                        |
    // ------------------------------------------------------------------------------------
    // | country              | c                           | c                           |
    // ------------------------------------------------------------------------------------
    // | organization         | o                           | o                           |
    // ------------------------------------------------------------------------------------
    // | pilotObject          | none                        | none                        |
    // ------------------------------------------------------------------------------------
    //
    // the 'organization' and 'country' objectclass can only be set when an 'o' or a 'c'
    // attribute is given. The other objectclasses will be added by default because the
    // haven't a mandatory attribute
    
    // fetch current registred objectclasses of user
    $oc_user = pql_get_attribute($ldap_linkid, $user, pql_get_define("PQL_ATTR_OBJECTCLASS"));
    
    // set all objectclasses to lowercase
    foreach($oc_user as $oc1){
		$oc[] = lc($oc1);
    }
    
    // add addressbook's default object classes if they are not present
    $default_oc = array("inetorgperson", "organizationalperson", "pilotobject");
    
    foreach($default_oc as $oc1){
		if(!in_array($oc1, $oc)){
			//$oc[] = $oc1;
		}
    }
    
    // check for object class dependencies
    echo var_dump($oc);
}

// }}}

// {{{ pql_get_next_ugidnumber(ldap, type)
// Returns the next free UID/GID number
function pql_get_next_ugidnumber($ldap, $type) {
    //: Get the minimum allowable ID.
	if($type == 'uid') {
		$attrib  = pql_get_define("PQL_ATTR_QMAILUID");
		$minimum = pql_get_define("PQL_CONF_MINIMUM_UIDNUMBER");
	} else {
		$attrib = pql_get_define("PQL_ATTR_QMAILGID");
		$minimum = pql_get_define("PQL_CONF_MINIMUM_GIDNUMBER");
	}

	// Just incase it's not defined, we set the minimum UID/GID
	// number availible to a safe value.
	if(!is_array($minimum))
	  $minimum = array(100);

	$min_id_val = 100;			// Set an absolute min so no one becomes root.
    foreach ($minimum as $min) {
	  if ($min > $min_id_val) {
		$min_id_val = $min;
	  }
	}

    //: Make a list of all the already used IDs.
    $filter = "(&($attrib=*)(objectclass=posixaccount))";

	$ids = array();
	foreach($_SESSION["BASE_DN"] as $dn) {
		$results = pql_search($ldap->ldap_linkid, $dn, $filter);
		for($i=0; $i < count($results); $i++) {
		  if ($results[$i][$attrib][0] >= $min_id_val) {
		    $ids[] = $results[$i][$attrib][0];
		  }
		}
	}

	//: Look for a hole in the ID list.
	$found_id = null;
	if (empty($ids)) {
	  $found_id = $min_id_val;
	} else {
	  sort($ids, SORT_NUMERIC);
	  $prev_id = $ids[0];
	  foreach ($ids as $id) {
		if ($id > $prev_id + 1) {
		  $found_id = $prev_id + 1;
		  break;
		}
		$prev_id = $id;
	  }

	  if (!isset($found_id)) {
		$found_id = end($ids) + 1;
	  }
	}

    return $found_id;
}

// }}}

// {{{ pql_get_next_username(ldap, domain)
// Returns the next free UID/GID number
function pql_get_next_username($ldap, $domain) {
	// Get the prefix from the branch
	$prefix = pql_get_attribute($ldap->ldap_linkid, $domain, pql_get_define("PQL_ATTR_USERNAME_PREFIX"));

	$suffix_length = pql_get_attribute($ldap->ldap_linkid, $domain, pql_get_define("PQL_ATTR_USERNAME_PREFIX_LENGTH"));
	if(!$suffix_length)
	  $suffix_length = "%03d";
	else
	  $suffix_length = "%0".$suffix_length."d";

	if($prefix) {
		// Under what namingContexts is this domain/branch located?
		$rootdn = pql_get_rootdn($domain, 'pql_get_next_username'); $rootdn = urldecode($rootdn);

		// Setup the LDAP search filter
		$filter = pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $rootdn) . '=' . $prefix . '*';

		// Search the whole database for all users with this prefix
		foreach($_SESSION["BASE_DN"] as $dn) {
			$dn  = urldecode($dn);
			$sr  = ldap_search($ldap->ldap_linkid, $dn, $filter, array(pql_get_define("PQL_ATTR_UID")));
			$tmp = ldap_get_entries($ldap->ldap_linkid, $sr) or pql_format_error(1);
			for($i=0; $i < $tmp["count"]; $i++)
			  $USERS[] = $tmp[$i][pql_get_define("PQL_ATTR_UID")][0];
		}

		if(!$USERS) {
			$suffix = sprintf("$suffix_length", 1);
		} else {
			// We have previous values in the DB. 
			sort($USERS);
			$nr  = eregi_replace($prefix, "", $USERS[count($USERS)-1]);

			$suffix = sprintf("$suffix_length", $nr + 1);
		}

		return($prefix.$suffix);
	}
}

// }}}

// {{{ pql_get_valid_shells()
// Load list of allowed shells from /etc/shells
function pql_get_valid_shells() {
	$fp = fopen("/etc/shells", "r");
	while (!feof ($fp)) {
		$buffer = fgets($fp, 4096);
		$shell = split(" ", $buffer);
		
		if(!eregi("^#", $shell[0]) and !eregi("^$", $shell[0])) {
			$shells[] = rtrim($shell[0]);
		}
	}
	fclose ($fp);
	asort($shells);

	return $shells;
}
// }}}

// {{{ pql_get_mx(domainname)
function pql_get_mx($domainname) {
	$mx = array();

	$res = getmxrr($domainname, $rec, $weight);
	if(count($rec) > 0) {
		// Take the MX with _LOWEST_ priority/weight.
		asort($weight); $old_prio = 65555;
		foreach($weight as $key => $prio) {
			if($prio < $old_prio) {
				$old_prio = $prio; $prio_key = $key;
			}
		}
		$mx["dns"] = $rec[$prio_key];
	}

	// It's possible that we have a QmailLDAP/Controls object as well...
	if(pql_get_define("PQL_CONF_CONTROL_USE")) {
		// Look for a qmailControl object which lists this domain...
		
		// Initiate a connection to the QmailLDAP/Controls DN
		$_pql_control = new pql_control($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);
		if($_pql_control->ldap_linkid) {
			$filter = "(&(objectclass=qmailControl)(".pql_get_define("PQL_ATTR_LOCALS")."=$domainname))";
			$info = pql_search($_pql_control->ldap_linkid, $_SESSION["USER_SEARCH_DN_CTR"], $filter);
			for($i=0; is_array($info[$i]); $i++) {
				if($info[$i]["cn"])
				  if(is_array($info[$i]["cn"]))
					 $mx["qlc"][] = $info[$i]["cn"][0];
				  else
					 $mx["qlc"][] = $info[$i]["cn"];
			}

			// Make sure we don't use it any more than we have to...
			ldap_close($_pql_control->ldap_linkid);
		}
	}

	return($mx);
}
// }}}

// {{{ pql_password_hash(password, hash)
function pql_password_hash($password, $hash) {
    // creates a hash of a password
    // - {SSHA}askdfjklödjfnddnu
    // - {SHA}kldfaldkjadsfdsad=
    // - {MD5}lkasjfdndiasdfee=
    // - {crypt}cryptedpassword
    // - {KERBEROS}principal@realm
    // - cleartextpasswort

	if(!function_exists("mhash") and ($hash = "{MD5}")) {
		// We do not have access to MD5 encryption - force {CRYPT}!
		unset($hash);

		$log = date("M d H:i:s");
		$log .= "MD5 encryption (mhash) not compiled into PHP. Please review the file doc/INSTALL. Falling back to using {CRYPT}";
		@error_log($log, 3, "phpQLadmin.log");
	}

    switch($hash){
      case "{SSHA}":
		// SHA1 hashing (salted)
		if(function_exists("mhash"))
		  return "{SSHA}" . base64_encode(mhash(MHASH_SHA1, $password, uniqid(microtime())));

      case "{SHA}":
		// SHA1 hashing
		if(function_exists("mhash"))
		  return "{SHA}" . base64_encode(mhash(MHASH_SHA1, $password));

      case "{MD5}":
		// MD5 hashing
		if(function_exists("mhash"))
		  return "{MD5}" . base64_encode(mhash(MHASH_MD5, $password));

      case "{CLEAR}":
		// Password in the clear (JUCK! :)
		return $password;

      case "{KERBEROS}":
		// Kerberos V mapping
		return $hash . $password;

      default:
		$salt_chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789./"; 
		$max_idx = strlen($salt_chars) - 1; 
		$salt = $salt_chars[rand(0, $max_idx)] . $salt_chars[rand(0, $max_idx)]; 

		return "{crypt}" . crypt($password, $salt);
    }
}
// }}}

// {{{ pql_generate_password(void)
function pql_generate_password() {
	$amount = 8;

	for(;;) {
		$pwd = NULL; $o = NULL;

		for($x=0; $x < $amount; ) {
			$y = rand(1, 1000);
			if(($y > 350) and ($y < 601)) $d = chr(rand(48, 57));
			if($y < 351) $d=chr(rand(65, 90));
			if($y > 600) $d=chr(rand(97, 122));
			if($d != $o) {
				$o = $d;
				$pwd .= $d;
				$x++;
			}
		}

		if(ereg("^[a-zA-Z]{1}([a-zA-Z]+[0-9][a-zA-Z]+)+", $pwd))
		  break;
	}

	return($pwd);
}
// }}}

// {{{ pql_check_email(email)
function pql_check_email($email) {
    $split = split("@", $email);
    
    if(count($split) != 2)
	  // does not contain @ or has to much of it
	  return false;
    
    // check user part
    if(!preg_match("/^[a-z0-9-+=#]+([\._a-z0-9-+=#]?[a-z0-9]+)*$/i", $split[0]))
	  // user part is not valid:
	  // - must start with a valid character (a-Z, 0-9)
	  // - may contain a special char (. _ + = # -), but not at start and end
	  //   and following another special char
	  // - must end with a valid character (a-Z, 0-9)
	  // - must contain at least 1 character
	  return false;
    
    // check host part
	return pql_check_hostaddress($split[1]);
}
// }}}

// {{{ pql_check_hostaddress(host, force_dot)
function pql_check_hostaddress($host, $force_dot = 0) {
    // checks an fqdn - if force_dot is true, it must contain a dot
    if($force_dot)
	  return preg_match("/^([0-9a-z-]+\.[0-9a-z-]+)+$/i", $host);
	else
	  return preg_match("/[^0-9a-z]/i", $host);
}
// }}}

// {{{ pql_execute(command, hide=true)
function pql_execute($command, $hide=true) {
	$command = "(" . escapeshellcmd($command) . ") 2>&1";

	if(!$hide)
	  echo "\n<pre>\n----\n";

	flush(passthru($command, $ret));

	if($ret)
	  $code = '=> <b><u>FAILED!!</u></b>';
	else
	  $code = '=> <b>SUCCESS</b>';

	if(!$hide)
	  echo "----\nreturn value: $ret $code\n</pre>";

	return $ret;
}
// }}}

// {{{ pql_ldap_accountstatus(status)
// Returns text of account-status
function pql_ldap_accountstatus($status) {
    global $LANG;

    // active [default]
    // nopop		(not availible in newer QmailLDAP it seems).
	// noaccess
    // disabled
	// deleted
    
    switch($status) {
      case "nopop":
		return $LANG->_('POP locked');

	  case "noaccess":
		return $LANG->_('Locked/Noaccess');

      case "disabled":
		return $LANG->_('Locked/Disabled');

	  case "deleted":
		return $LANG->_('Deleted');

      default:
		return $LANG->_('Active');
    }
}
// }}}

// {{{ pql_ldap_deliverymode(mode)
// Returns text of delivery mode
function pql_ldap_deliverymode($mode) {
    global $LANG;

	if($_SESSION["NEW_STYLE_QUOTA"]) {
		// multi field entries of these keywords
		// - (normal)	put message into maildir/mbox, plus forward and program delivery
		// - noforward	do not forward (ignores forwarding entries in ldap and .qmail)
		// - nolocal	do not put message into maildir/mbox (ignores also .qmail)
		// - noprogram	do not do program deliveries (ignores deliveryprogrampath, .qmail)
		// - reply		send an auto_reply mail with text from mailReplyText
		switch($mode) {
		  case "normal":
			return $LANG->_('Save in local mailbox, allow forwarding and program delivery');

		  case "noforward":
			return $LANG->_('Save in local mailbox only (no forwarding)');

		  case "nolocal":
			return $LANG->_('Do not save in local mailbox, allow forwarding and program delivery');

		  case "noprogram":
			return $LANG->_('Do not allow program delivery (ignores deliveryProgramPath and .qmail)');

		  case "reply":
			return $LANG->_('Send an automatic reply mail');
		}
	} else {
		// normal [default]
		// forwardonly
		// nombox
		// localdelivery
		// reply
		// echo
		switch($mode) {
		  case "forwardonly":
			return $LANG->_('Only forward');
			
		  case "nombox":
			return $LANG->_('No local mailbox');
			
		  case "localdelivery":
			return $LANG->_('Save in local mailbox');
			
		  case "reply":
			return $LANG->_('Send an automatic reply mail');
			
		  case "echo":
			return $LANG->_('Echo to console (tricky)');
			
		  default:
			return $LANG->_('Normal');
		}
	}
}

// }}}

// {{{ pql_split_oldvalues(value)
function pql_split_oldvalues($value) {
	// (Possibly) split the old value array
	if(eregi(" ", $value)) {
		$values = split(" ", $value);
	} elseif(eregi(",", $value)) {
		$values = split(",", $value);
	} elseif($value) {
		$values[] = $value;
	}

	if(is_array($values)) {
		asort($values);
		return($values);
	} else
	  return false;
}
// }}}

// {{{ pql_missing_objectclasses(linkid, dn, entry)
function pql_missing_objectclasses($linkid, $dn, $entry) {
	// Get objectclasses from the LDAP server, finding the objectclass
	// that MUST (or if non is found, a MAY) have the attribute we're
	// trying to modify.
	$objectclasses = pql_get_subschema($linkid, 'objectclasses');
	if(is_array($objectclasses)) {
		foreach($entry as $attrib => $val) {
			// For each of the attributes (exept the objectclass one)
			// in the object, let's see if we can find it's objectclass.

			if($attrib != pql_get_define("PQL_ATTR_OBJECTCLASS")) {
				foreach($objectclasses as $key => $oc) {
					// Go through the MUST attributes
					for($i=0; $i < $oc[MUST]['count']; $i++)
					  if(lc($oc[MUST][$i]) == lc($attrib))
						$objectclass[] = $key;
					
					if(!is_array($objectclass)) {
						// Didn't find a MUST, try to find a MAY attribute
						for($i=0; $i < $oc[MAY]['count']; $i++)
						  if(lc($oc[MAY][$i]) == lc($attrib))
							$objectclass[] = $key;
					}
				}
			}
		}

		if(is_array($objectclass)) {
			// We've found objectclass(es) that provide the attributes in the object.
			for($i=0; $existing[$i]; $i++)
			  $objectclass[] = $existing[$i];

			return($objectclass);
		}
	} else
	  return false;
}
// }}}

// {{{ pql_recreate_dnsttl(dn)
function pql_recreate_dnsttl($dn) {
  // De urldecode (part of) the DNS DN.
  // We're missing the '+' between dNSTTL and relativeDomainName
  // (which urldecode() removes!):
  // 
  // dNSTTL=3600+relativeDomainName=@,dc=domain,dc=tld,ou=DNS,o=Branch,c=SE
  $dn_parts = explode(',', $dn);
  
  // Create the new DN with all but the very first part.
  $count = count($dn_parts);
  for($i=1; $i <= $count; $i++) {
	$newdn .= $dn_parts[$i];
	if($dn_parts[$i+1])
	  $newdn .= ",";
  }
  
  // Add the missing part
  $tmp   = split(' ', $dn_parts[0]);
  $newdn = $tmp[0]."+".$tmp[1].",".$newdn;
  
  return($newdn);
}
// }}}

/*
 * Local variables:
 * mode: php
 * tab-width: 4
 * End:
 */
?>
