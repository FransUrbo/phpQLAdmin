<?php
// control attribute plugin for
// smtproutes (max number of bytes in message)
//
// $Id: attrib.control.smtproutes.inc,v 2.17 2004-02-14 14:25:36 turbo Exp $

function smtproutes_check($type) {
	global $error, $domain, $relay, $port, $view, $LANG;
	global $rootdn, $type, $set, $domainname;

	// check all domains
	if(is_array($domainname)) {
		foreach($domainname as $key => $dom) {
			if($dom) {
				if(preg_match("/[^0-9a-z-\.]+/i",$dom)) {
					$error["domain_$key"] = $LANG->_('Invalid');
					$is_error = true;
				}
			}
		}
	}
	
	// relay hosts
	if(is_array($relay)) {
		foreach($relay as $key => $rel) {
			if($rel != "" and !pql_check_hostaddress($rel, true)) {
				$error["relay_$key"] = $LANG->_('Invalid');
				$is_error = true;
			}
		}
	}
	
	// ports - check only if we're not deleting a specific domain
	if(is_array($port) and ($type != 'del')) {
		foreach($port as $key => $prt) {
			if(preg_match("/[^0-9]+/i", $prt)) {
				$error["port_$key"] = $LANG->_('Invalid');
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
										   pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $host . "," . $_SESSION["USER_SEARCH_DN_CTR"],
										   pql_get_define("PQL_GLOB_ATTR_SMTPROUTES"));
		
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
	global $_pql_control, $smtproutes, $view, $LANG;
	global $rootdn, $domain, $attrib, $type, $set;

	// init data
	smtproutes_init($host);

	if(is_array($smtproutes)) {
		foreach($smtproutes as $route) {
			// domain
			if($route[0] == "")
			  $route[0] = "&lt; ".$LANG->_('All', 4)." &gt;";
			
			// relay host
			if($route[1] == "")
			  $route[1] = $LANG->_('MX record');
			
			// port
			if($route[2] == "")
			  $route[2] = "25";
			
			$routes[] = $route;
		}
	}
	
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">smtproutes (<?=$LANG->_('Defined SMTP routes')?>)</th>
      <tr class="title">
        <td><?=$LANG->_('Source domain')?></td>
        <td>&nbsp;</td>
        <td><?=$LANG->_('Relay host')?></td>
        <td>&nbsp;</td>
        <td><?=$LANG->_('Port')?></td>
        <td></td>
      </tr>
<?php
	if(!is_array($routes)) {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td colspan="5"><?=$LANG->_('No routes defined')?></td>
      </tr>
<?php
	} else {
		foreach($routes as $route) {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td><?=pql_maybe_idna_decode($route[0])?></td>
        <td><b>=&gt;</b></td>
        <td><?=pql_maybe_idna_decode($route[1])?></td>
        <td><b>:</b></td>
        <td><?=pql_maybe_idna_decode($route[2])?></td>
        <td>
          <a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=smtproutes&type=modify&set=<?=$route[0].":".$route[1].":".$route[2]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Change options')?>"></a>
          <a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=smtproutes&type=del&submit=1&set=<?=$route[0].":".$route[1].":".$route[2]?>"><img src="images/del.png"  width="12" height="12" border="0" alt="<?=$LANG->_('Delete options')?>"></a>
        </td>
      </tr>

<?php
		} // end foreach
	} // end if is_array
?>
      <tr class="subtitle">
        <td><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=smtproutes&type=add"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Add SMTP route')?></a></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
      </tr>
    </th>
  </table>

<?php
}

function smtproutes_print_form() {
	global $attrib, $error, $domain, $relay, $port, $routecount, $mxhost, $view, $LANG;
	global $rootdn, $type, $set, $_pql_control;

	if($attrib && $rootdn && $set)
	  // We're called from the domain details page -> the domain name is in $set
	  $domainname[0] = $set;
	elseif($attrib && $set && $mxhost) {
		// We're called from the domain details page - change value
		$tmp = split(":", $set); unset($set);

		$ch_domainname = $tmp[0];
		$ch_relay      = $tmp[1];
		$ch_port       = $tmp[2];
	} elseif($type && $mxhost)
	  // We're called from the control details page -> add new value
	  unset($domainname);
	else
	  // We're called from the control details page -> the domain name is in $domain
	  $domainname = $domain;

	if(($routecount < count($domainname) - 1) and count($domainname) != 0)
	  $routecount = count($domainname) - 1;

	// --------------------------------------------------------------------
	// There's acctually TWO 'completly' different forms here...
	//
	// One is when we're called from the domain details ('Control Options').
	// The intention with this is that we want to route the domain to <empty>
	// mailserver. We know the domain name, and we can find out the mailserver
	// by getting all QmailLDAP/Controls object from the USER_SEARCH_DN_CTR
	// DN. Show a select menu and fill in a default port (25).
	//
	// The other form is when we're called from the Controls frame/details.
	// The intention with this is that we want to route <empty> domain
	// to the known mailserver.
	// We know the mailserver and we fill in a default port (25). All we
	// need is a source domain (the domain to route).
	// --------------------------------------------------------------------

	// Default port...
	if(!$port[0])
	  $port[0] = 25;

	if($type != 'del') {
		if(!$mxhost)
		  // Get all the QmailLDAP/Controls hosts
		  $hosts  = pql_control_get_hosts($_pql_control->ldap_linkid, $_SESSION["USER_SEARCH_DN_CTR"]);
	} else {
		// Get all hosts which have this domain in 'attrib'
		$hosts = pql_control_search_attribute($_pql_control->ldap_linkid,
											  $_SESSION["USER_SEARCH_DN_CTR"],
											  $attrib, $set."*", 1);

		// Convert the DN into a host list
		for($i=0; $hosts[$i]; $i++) {
			$host = split(',', $hosts[$i]);
			$host = eregi_replace(pql_get_define("PQL_GLOB_ATTR_CN") . "=", "", $host[0]);
			$newhosts[] = $host;
		}

		$hosts = $newhosts;
	}
?>
        <form action="<?php echo $_SERVER["PHP_SELF"] ?>" method="post">
<?php if($attrib && !$rootdn && !$set && (($type == 'add') || ($type == 'modify'))) {
		// We're adding ONE value, but we want to remember the previous

		for($i=0; $i < count($domain); $i++) {
?>
          <!-- Relay #<?=$i?>: <?=$domain[$i]?>:<?=$relay[$i]?>:<?=$port[$i]?> -->
          <input type="hidden" name="domainname[<?=$i?>]" value="<?=$domain[$i]?>">
          <input type="hidden" name="relay[<?=$i?>]"      value="<?=$relay[$i]?>">
          <input type="hidden" name="port[<?=$i?>]"       value="<?=$port[$i]?>">

<?php	}

		if($ch_domainname && $ch_relay && $ch_port) {
			// Remember what value to change
			for($i=0; $i < count($domain); $i++) {
				if(eregi($ch_domainname, $domain[$i]))
				  $change = $i;
			}
		}

		$counter = $i;
      } else
		$counter = 0;

	if(isset($change)) {
		$objnr = $change;

		$domainname[$objnr] = $ch_domainname;
	} else {
		$objnr = $counter;
		unset($domain); unset($relay); unset($port);
	}
	
	if($ch_domainname && $ch_relay && $ch_port) {
?>
          <!-- This is the number of the relay (see above) we are changing -->
          <input type="hidden" name="change"        value="<?=$change?>">

<?php } ?>
          <table cellspacing="0" cellpadding="3" border="0">
            <th colspan="3" align="left">smtproutes (<?=$LANG->_('SMTP routes')?>)</th>
              <!-- A --------------------------------- -->
              <tr class="title">
                <td><?=$LANG->_('Source domain')?></td>
<?php if($attrib && $rootdn && $set) { ?>
                <td>QmailLDAP/Controls</td>
<?php } else { ?>
                <td><?=$LANG->_('Relay host')?></td>
<?php } ?>
<?php if($type != 'del') { ?>
                <td><?=$LANG->_('Port')?></td>
<?php } ?>
              </tr>

              <!-- B --------------------------------- -->
              <tr class="<?php pql_format_table(); ?>">
                <td><?php echo pql_format_error_span($error["domainname_$counter"]); ?><input type="text" name="domainname[<?=$counter?>]" value="<?=$domainname[$objnr]?>">&nbsp;&nbsp;<b><?php if($type != 'del') { ?>=><?php } ?></b></td>
<?php if($attrib && $rootdn && $set) { ?>
                <td>
                  <select name="relay[]" size="1" multiple>
                    <option value=""><?=$LANG->_('Please select a host')?></option>
<?php	for($i=0; $hosts[$i]; $i++) { ?>
                    <option value="<?=$hosts[$i]?>"><?=$hosts[$i]?></option>
<?php	} ?>
                  </select>
                </td>
<?php } else { ?>
                <td><?php echo pql_format_error_span($error["relay_$counter"]); ?><input type="text" name="relay[<?=$counter?>]" value="<?=$relay[$objnr]?>">&nbsp;&nbsp;<b>:</b></td>
<?php } ?>
<?php if($type != 'del') { ?>
                <td><?php echo pql_format_error_span($error["port_$counter"]); ?><input type="text" size="4" maxlength="4" name="port[<?=$counter?>]" value="<?=$port[$objnr]?>"></td>
<?php } ?>
              </tr>

<?php for($i = 1, $j = $counter; $i <= $routecount; $i++, $counter++) { ?>
              <!-- C --------------------------------- -->
              <tr class="<?php pql_format_table(); ?>">
                <td><?php echo pql_format_error_span($error["domainname_$i"]); ?><input type="text" name="domainname[<?=$i?>]" value="<?=$domainname[$i]?>">&nbsp;&nbsp;<b>=></b></td>
                <td><?php echo pql_format_error_span($error["relay_$i"]); ?><input type="text" name="relay[<?=$i?>]" value="<?$relay[$i]?>">&nbsp;&nbsp;<b>:</b></td>
                <td><?php echo pql_format_error_span($error["port_$i"]); ?><input type="text" size="4" maxlength="4" name="port[<?=$i?>]" value="<?=$port[$i]?>"></td>
              </tr>

<?php } ?>
<?php if(!$attrib) { ?>
              <tr class="subtitle">
                <td colspan="3"><a href="<?=$_SERVER["PHP_SELF"]?>?attrib=<?=$attrib?>&routecount=<?=($routecount+1)?>&mxhost=<?=$mxhost?>">add additional route</a>(<?=$LANG->_('Please save first, changes will be lost', 4)?>)</td>
              </tr>

<?php } 

	  if($type != 'del') {
?>
              <!-- D --------------------------------- -->
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

          <input type="hidden" name="submit"        value="1">
          <input type="hidden" name="attrib"        value="<?=$attrib?>">
<?php if($mxhost) {            ?>          <input type="hidden" name="mxhost"        value="<?=$mxhost?>"><?php } ?>
<?php if($rootdn) {            ?>          <input type="hidden" name="rootdn"        value="<?=$rootdn?>"><?php } ?>
<?php if(!is_array($domain)) { ?>          <input type="hidden" name="domain"        value="<?=$domain?>"><?php } ?>
          <input type="hidden" name="type"          value="<?=$type?>">
          <input type="hidden" name="view"          value="<?=$view?>">
          <br>
          <input type="submit"                      value="<?=$LANG->_('Save')?>">
        </form>
<?php
}

function smtproutes_save($type, $host) {
	global $_pql_control, $attrib, $smtproutes, $error, $domain, $relay, $port, $view, $LANG;
	global $rootdn, $type, $domainname, $set, $change;
	$success = 0;

	switch($type) {
	  case "modify":
	  case "add":
		// save entries - combine attributes
		if($host) {
			// We're called from the control details page -> we know what object to update

			if($change >= 0) {
				// We're replacing a value with another - Delete the value in $change
				// and rearrange the array.
				unset($domainname[$change]);
				unset($relay[$change]);
				unset($port[$change]);

				foreach($domainname as $d)
				  $tmp[] = $d;
				$domainname = $tmp; unset($tmp);

				foreach($relay as $r)
				  $tmp[] = $r;
				$relay = $tmp; unset($tmp);

				foreach($port as $p)
				  $tmp[] = $p;
				$port = $tmp; unset($tmp);
			}

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
											 pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $host . "," . $_SESSION["USER_SEARCH_DN_CTR"],
											 pql_get_define("PQL_GLOB_ATTR_SMTPROUTES"),
											 $smtproutes))
			  $success = 1;
		} else {
			// We're called from the domain details page -> we must find out what object to update

			if($relay) {
				// Go through each QmailLDAP/Controls object, fetching old values
				for($i=0; $i < count($relay); $i++) {
					if($relay[$i]) {
						// fetch data from ldap server
						$values = pql_control_get_attribute($_pql_control->ldap_linkid,
															pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $relay[$i] . "," . $_SESSION["USER_SEARCH_DN_CTR"],
															pql_get_define("PQL_GLOB_ATTR_SMTPROUTES"));
						
						// Add the new value
						$values[] = $domainname[0].":".$relay[$i].":25";
						
						// Update the object
						if(pql_control_replace_attribute($_pql_control->ldap_linkid,
														 pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $relay[$i] . "," . $_SESSION["USER_SEARCH_DN_CTR"],
														 pql_get_define("PQL_GLOB_ATTR_SMTPROUTES"),
														 $values))
						  $success = 1;
					}
				}
			}
		}

		if($success)
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('smtproutes options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('smtproutes options'))) . ": " . pql_format_error(0);

		attribute_forward($msg);
		break;

	  case "del":
		// We'd like to remove a domain from one (or more) QmailLDAP/Controls objects
		// Go through each QmailLDAP/Controls object, fetching old values
		if($relay) {
			// We're called from the control details page -> we know what object to update

			for($i=0; $i < count($relay); $i++) {
				if($relay[$i]) {
					// fetch data from ldap server
					$values = pql_control_get_attribute($_pql_control->ldap_linkid,
														pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $relay[$i] . "," . $_SESSION["USER_SEARCH_DN_CTR"],
														pql_get_define("PQL_GLOB_ATTR_SMTPROUTES"));
					
					// Go through the values, removing the specific domain
					for($j=0; $values[$j]; $j++) {
						if(! eregi($domainname[0], $values[$j]))
						  $newvals[] = $values[$j];
					}
					
					// Update the object
					if(pql_control_replace_attribute($_pql_control->ldap_linkid,
													 pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $relay[$i] . "," . $_SESSION["USER_SEARCH_DN_CTR"],
													 pql_get_define("PQL_GLOB_ATTR_SMTPROUTES"),
													 $newvals))
					  $success = 1;
				}
			}
		} elseif(!$domain && !$rootdn && $attrib && $host && $set) {
			// We're deleting ONE value. $set contains the whole string, as it looks like in the object
			
			// Find the object(s) which contain this string
			$ctrls = pql_control_search_attribute($_pql_control->ldap_linkid, $_SESSION["USER_SEARCH_DN_CTR"],
												  "smtproutes", $set);
			
			// If it isn't an array, but it IS set -> make it an array!
			if($ctrls && !is_array()) {
				$tmp = $ctrls; unset($ctrls);
				$ctrls[] = $tmp;
			}
			
			foreach($ctrls as $ctrl) {
				// Retreive this QmailLDAP/Controls object current value(s)
				$routes = pql_control_get_attribute($_pql_control->ldap_linkid,
													$ctrl,
													pql_get_define("PQL_GLOB_ATTR_SMTPROUTES"));
				if(is_array($routes)) {
					// Go through the SMTP routes, remove the one in $relay
					foreach($routes as $route)
					  if(!eregi($set, $route))
						$newroutes[] = $route;
				}
				
				// Save this new array to the object
				if(pql_control_replace_attribute($_pql_control->ldap_linkid,
												 $ctrl,
												 pql_get_define("PQL_GLOB_ATTR_SMTPROUTES"),
												 $newroutes))
				  $success = 1;
			}
		} elseif($domain && $set && $rootdn && $attrib) {
			// We're called from the domain details page -> we must find out what object to update

			// Find QmailLDAP/Controls object(s) which contain this domain in SMTPRoutes
			$ctrls  = pql_control_search_attribute($_pql_control->ldap_linkid,
												   $_SESSION["USER_SEARCH_DN_CTR"],
												   "smtproutes",
												   $set."*");
			// If it isn't an array, but it IS set -> make it an array!
			if($ctrls && !is_array()) {
				$tmp = $ctrls; unset($ctrls);
				$ctrls[] = $tmp;
			}

			foreach($ctrls as $ctrl) {
				// Retreive this QmailLDAP/Controls object current value(s)
				$routes = pql_control_get_attribute($_pql_control->ldap_linkid,
													$ctrl,
													pql_get_define("PQL_GLOB_ATTR_SMTPROUTES"));
				if(is_array($routes)) {
					// Go through the SMTP routes, remove the one in $set
					foreach($routes as $route)
					  if(!eregi($set, $route))
						$newroutes[] = $route;
				}

				// Save this new array to the object
				if(pql_control_replace_attribute($_pql_control->ldap_linkid,
												 $ctrl,
												 pql_get_define("PQL_GLOB_ATTR_SMTPROUTES"),
												 $newroutes))
				  $success = 1;
			}
		}

		if($success)
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('smtproutes options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('smtproutes options'))) . ": " . pql_format_error(0);

		attribute_forward($msg);
		break;

   	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}

function smtproutes_help() {
    global $LANG;
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

function smtproutes_help_cr() {
	global $LANG;
	echo $LANG->_('Help text taken from qmail man pages');
}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
