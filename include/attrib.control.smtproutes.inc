<?php
// control attribute plugin for
// smtproutes (max number of bytes in message)
//
// $Id: attrib.control.smtproutes.inc,v 2.10 2003-06-28 09:21:06 turbo Exp $

function smtproutes_check($type) {
	global $error, $domain, $relay, $port, $view;
	global $rootdn, $type, $set, $domainname;

	// check all domains
	if(is_array($domainname)) {
		foreach($domainname as $key => $dom) {
			if($dom) {
				if(preg_match("/[^0-9a-z-\.]+/i",$dom)) {
					$error["domain_$key"] = PQL_LANG_INVALID;
					$is_error = true;
				}
			}
		}
	}

	// relay hosts
	if(is_array($relay)) {
		foreach($relay as $key => $rel) {
			if($rel != "" and !check_hostaddress($rel)) {
				$error["relay_$key"] = PQL_LANG_INVALID;
				$is_error = true;
			}
		}
	}

	// ports - check only if we're not deleting a specific domain
	if(is_array($port) and ($type != 'del')) {
		foreach($port as $key => $prt) {
			if(preg_match("/[^0-9]+/i", $prt)) {
				$error["port_$key"] = PQL_LANG_INVALID;
				$is_error = true;
			}
		}
	}
	
	if($is_error)
	  return false;
	
	return true;
}

function smtproutes_init($host) {
	// init values
	global $_pql_control, $smtproutes, $domain, $relay, $port, $view;
	global $rootdn, $attrib, $type, $set;

	if($host) {
		// fetch data from ldap server
		$value = pql_control_get_attribute($_pql_control->ldap_linkid,
										   "cn=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
										   "smtproutes");
		
		if(is_null($value))
		  return true;

		foreach($value as $route)
		  $smtproutes[] = split(":", $route);
		
		foreach($smtproutes as $route) {
			$domain[] = $route[0];
			$relay[] = $route[1];
			$port[] = $route[2];
		}
	} else
	  // We're called from the domain details page,
	  // get SMTP routes later (in the print_form() func).
	  return true;
}

function smtproutes_print_view($host) {
	global $_pql_control, $smtproutes, $view;
	global $rootdn, $domain, $attrib, $type, $set;

	// init data
	smtproutes_init($host);

	if(is_array($smtproutes)) {
		foreach($smtproutes as $route) {
			// domain
			if($route[0] == "")
			  $route[0] = "&lt; all &gt;";
			
			// relay host
			if($route[1] == "")
			  $route[1] = "MX-record";
			
			// port
			if($route[2] == "")
			  $route[2] = "25";
			
			$routes[] = $route;
		}
	}
	
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">smtproutes (Defined SMTP routes)</th>
      <tr class="title">
        <td>Source domain</td>
        <td>&nbsp;</td>
        <td>Relay host</td>
        <td>&nbsp;</td>
        <td>Port</td>
      </tr>
<?php
	if(!is_array($routes)) {
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td colspan="5">no routes defined</td>
      </tr>
<?php
	} else {
		foreach($routes as $route) {
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td><?php echo $route[0]; ?></td>
        <td><b>=&gt;</b></td>
        <td><?php echo $route[1]; ?></td>
        <td><b>:</b></td>
        <td><?php echo $route[2]; ?></td>
      </tr>
<?php
		} // end foreach
	} // end if is_array
?>
      <tr class="subtitle">
        <td colspan="5"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=smtproutes"><img src="images/edit.png" width="12" height="12" border="0"> change options</a></td>
      </tr>
    </th>
  </table>

<?php
}

function smtproutes_print_form() {
	global $attrib, $PHP_SELF, $error, $domain, $relay, $port, $routecount, $mxhost, $view;
	global $rootdn, $domain, $attrib, $type, $set, $view, $_pql_control;

	if($attrib && $rootdn && $set)
	  // We're called from the domain details page -> the domain name is in $set
	  $domainname[0] = $set;
	else
	  // We're called from the control details page -> the domain name is in $domain
	  $domainname = $domain;

	if(($routecount < count($domainname) - 1) and count($domainname) != 0)
	  $routecount = count($domainname) - 1;

	// --------------------------------------------------------------------
	// There's acctually TWO 'completly' different forms here...
	//
	// One is when we're called from the domain details. The intention
	// with this is that we want to route a domain to <empty> mailserver.
	// We know the domain name, and we can find out the mailserver by
	// getting all QmailLDAP/Controls object from the USER_SEARCH_DN_CTR
	// DN. Show a select menu and fill in a default port (25).
	//
	// The other form is when we're called from the Controls frame/details.
	// The intention with this is that we want to route <empty> domain
	// to the known mailserver.
	// We know the mailserver and we fill in a default port (25). All we
	// need is a source domain (the domain to route).
	// --------------------------------------------------------------------

	// Default port...
	$port[0] = 25;

	if($type != 'del')
	  // Get all the QmailLDAP/Controls hosts
	  $hosts = pql_control_get_hosts($_pql_control->ldap_linkid, $GLOBALS["USER_SEARCH_DN_CTR"]);
	else {
		// Get all hosts which have this domain in 'attrib'
		$hosts = pql_control_search_attribute($_pql_control->ldap_linkid,
											  $GLOBALS["USER_SEARCH_DN_CTR"],
											  $attrib, $set."*", 1);

		// Convert the DN into a host list
		for($i=0; $hosts[$i]; $i++) {
			$host = split(',', $hosts[$i]);
			$host = eregi_replace("cn=", "", $host[0]);
			$newhosts[] = $host;
		}

		$hosts = $newhosts;
	}
?>
        <form action="<?php echo $PHP_SELF ?>" method="post">
          <table cellspacing="0" cellpadding="3" border="0">
            <th colspan="3" align="left">smtproutes (SMTP routes)</th>
              <tr class="title">
                <td>Source domain</td>
<?php if($attrib && $rootdn && $set) { ?>
                <td>QmailLDAP/Controls</td>
<?php } else { ?>
                <td>Relay host</td>
<?php } ?>
<?php if($type != 'del') { ?>
                <td>Port</td>
<?php } ?>
              </tr>

              <tr class="<?php table_bgcolor(); ?>">
                <td><?php echo format_error($error["domainname_0"]); ?><input type="text" name="domainname[0]" value="<?php echo $domainname[0]; ?>">&nbsp;&nbsp;<b><?php if($type != 'del') { ?>=><?php } ?></b></td>
<?php if($attrib and is_array($hosts)) { ?>
                <td>
                  <select name="relay[]" size="1" multiple>
                    <option value="">Please select a host</option>
<?php	for($i=0; $hosts[$i]; $i++) { ?>
                    <option value="<?=$hosts[$i]?>"><?=$hosts[$i]?></option>
<?php	} ?>
                  </select>
                </td>
<?php } else { ?>
                <td><?php echo format_error($error["relay_0"]); ?><input type="text" name="relay[0]" value="<?php echo $relay[0]; ?>">&nbsp;&nbsp;<b>:</b></td>
<?php } ?>
<?php if($type != 'del') { ?>
                <td><?php echo format_error($error["port_0"]); ?><input type="text" size="4" maxlength="4" name="port[0]" value="<?php echo $port[0]; ?>"></td>
<?php } ?>
              </tr>

<?php for($i = 1; $i <= $routecount; $i++){ ?>
              <tr class="<?php table_bgcolor(); ?>">
                <td><?php echo format_error($error["domainname_$i"]); ?><input type="text" name="domainname[<?php echo $i; ?>]" value="<?php echo $domainname[$i]; ?>">&nbsp;&nbsp;<b>=></b></td>
                <td><?php echo format_error($error["relay_$i"]); ?><input type="text" name="relay[<?php echo $i; ?>]" value="<?php echo $relay[$i]; ?>">&nbsp;&nbsp;<b>:</b></td>
                <td><?php echo format_error($error["port_$i"]); ?><input type="text" size="4" maxlength="4" name="port[<?php echo $i; ?>]" value="<?php echo $port[$i]; ?>"></td>
              </tr>

<?php } ?>
<?php if(!$attrib) { ?>
              <tr class="subtitle">
                <td colspan="3"><a href="<?=$PHP_SELF?>?attrib=<?=$attrib?>&routecount=<?=($routecount+1)?>&mxhost=<?=$mxhost?>">add additional route</a>(please save first, changes will be lost)</td>
              </tr>

<?php } ?>

<?php if($type != 'del') { ?>
              <tr class="subtitle">
                <td colspan="3"><img src="images/info.png" width="16" height="16" border="0">If the source domain is empty, the route affects to all hosts. If relay host is empty, qmail will take the MX record of the domain to relay. Use port if the smtp port of the relay host is not 25.<?php if(!$attrib) { ?> Leave domain and relay empty to delete a route.<?php } ?></td>
              </tr>
<?php } else { ?>
              <tr class="subtitle">
                <td colspan="3"><img src="images/info.png" width="16" height="16" border="0">You have selected to remove the domain <b><?=$domainname[0]?></b> from <i>smtproutes</i>. Please select host(s) to remove it from. All other entries in the object(s) will be preserved</td>
              </tr>
<?php } ?>
            </th>
          </table>

          <input type="hidden" name="submit" value="1">
          <input type="hidden" name="attrib" value="<?php echo $attrib; ?>">
<?php if($attrib) { ?>
          <input type="hidden" name="rootdn" value="<?=$rootdn?>">
          <input type="hidden" name="domain" value="<?=$domain?>">
          <input type="hidden" name="type"   value="<?=$type?>">
<?php } else { ?>
          <input type="hidden" name="mxhost" value="<?=$mxhost?>">
<?php } ?>
          <input type="hidden" name="view"   value="<?=$view?>">
          <br>
          <input type="submit" value="<?php echo PQL_LANG_SAVE; ?>">
        </form>
<?php
}

function smtproutes_save($type, $host) {
	global $_pql_control, $attrib, $smtproutes, $error, $domain, $relay, $port, $view;
	global $rootdn, $type, $domainname;
	$success = 0;

	switch($type) {
	  case "modify":
	  case "add":
		// save entries - combine attributes
		if($host) {
			// We're called from the control details page -> we know what object to update

			for($i=0; $i < count($domainname); $i++) {
				// if empty, delete route
				if($domainname[$i] == "" and $relay[$i] == "")
				  continue;
				
				// if port or relay is not defined, leave out
				if($port[$i] == "" or $relay[$i] == "")
				  $tmp = array($domainname[$i], $relay[$i]);
				else
				  $tmp = array($domainname[$i], $relay[$i], $port[$i]);
				
				$route = join(":", $tmp);
				$smtproutes[] = $route;
			}
			
			// Update the object
			if(pql_control_replace_attribute($_pql_control->ldap_linkid,
											 "cn=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
											 "smtproutes", $smtproutes))
			  $success = 1;
		} else {
			// We're called from the domain details page -> we must find out what object
			// to update. It's in the $relay array.

			// Go through each QmailLDAP/Controls object, fetching old values
			for($i=0; $i < count($relay); $i++) {
				if($relay[$i]) {
					// fetch data from ldap server
					$values = pql_control_get_attribute($_pql_control->ldap_linkid,
														"cn=" . $relay[$i] . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
														"smtproutes");

					// Add the new value
					$values[] = $domainname[0].":".$relay[$i].":25";

					// Update the object
					if(pql_control_replace_attribute($_pql_control->ldap_linkid,
													 "cn=" . $relay[$i] . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
													 "smtproutes", $values))
					  $success = 1;
				}
			}
		}

		if($success)
		  $msg = "Successfully saved smtproutes options";
		else
		  $msg = "Failed saving smtproutes: " . pql_ldap_error($_pql_control->ldap_linkid);

		attribute_forward($msg);
		break;

	  case "del":
		// We'd like to remove a domain from one (or more) QmailLDAP/Controls objects
		// Go through each QmailLDAP/Controls object, fetching old values
		for($i=0; $i < count($relay); $i++) {
			if($relay[$i]) {
				// fetch data from ldap server
				$values = pql_control_get_attribute($_pql_control->ldap_linkid,
													"cn=" . $relay[$i] . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
													"smtproutes");
				
				// Go through the values, removing the specific domain
				for($j=0; $values[$j]; $j++) {
					if(! eregi($domainname[0], $values[$j]))
					  $newvals[] = $values[$j];
				}

				// Update the object
				if(pql_control_replace_attribute($_pql_control->ldap_linkid,
												 "cn=" . $relay[$i] . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
												 "smtproutes", $newvals))
				  $success = 1;
			}
		}

		if($success)
		  $msg = "Successfully saved smtproutes options";
		else
		  $msg = "Failed saving smtproutes: " . pql_ldap_error($_pql_control->ldap_linkid);

		attribute_forward($msg);
		break;

   	  default:
		die("unknown save type $type in " . __FILE__ . ", function save()");
	}
}

function smtproutes_help() {
?>
	Artificial SMTP routes.  Each route has the form
	domain:relay, without any extra spaces.  If domain
	matches host, qmail-remote will connect to relay, as if
	host had relay as its only MX.  (It will also avoid
	doing any CNAME lookups on recip.)  host may include a
	colon and a port number to use instead of the normal
	SMTP port, 25:
	<br><br>
			inside.af.mil:firewall.af.mil:26
	<br><br>
	relay may be empty; this tells qmail-remote to look up
	MX records as usual.  smtproutes may include wildcards:
	<br><br>
			.af.mil:<br>
			:heaven.af.mil
	<br><br>
	Here any address ending with .af.mil (but not af.mil
	itself) is routed by its MX records; any other address
	is artificially routed to heaven.af.mil.
	<br><br>
	The qmail system does not protect you if you create an
	artificial mail loop between machines.  However, you
	are always safe using smtproutes if you do not accept
	mail from the network.
<?php
}

function smtproutes_help_cr(){
?>
	help text taken from qmail man pages
<?php
}
/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
