<?php
// shows configuration of phpQLAdmin
// $Id: config_ldap.php,v 1.26 2007-03-12 10:09:39 turbo Exp $
//
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");

require("./left-head.html");
include($_SESSION["path"]."/header.html");

$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

if($_REQUEST["type"] == 'config') {
  echo "It's supposed to be information retreived from the cn=config backend, but it's not yet implemented...<br>";
  echo "Basically because ACL/ACI don't work properly yet in the LDAP server.";
  die();
} elseif($_REQUEST["type"] == 'accesslog') {
  if($_REQUEST["filter"]) {
	if(pql_get_define("PQL_CONF_DEBUG_ME")) {
	  echo "_REQUEST:";
	  printr($_REQUEST);
	}
	$filter = '(&';
	
	// Filter on type
	// NOTE: This is quite ugly, because there's a lot of possibilities with
	//       four choices!

	if($_REQUEST["filter_type_unbind"] or $_REQUEST["filter_type_search"] or $_REQUEST["filter_type_modify"] or $_REQUEST["filter_type_delete"])
	  $filter .= '(|';

	// BIND
	if($_REQUEST["filter_type_bind"])
	  $filter .= '(reqType=bind)';

	// UNBIND
	if($_REQUEST["filter_type_unbind"])
	  $filter .= '(reqType=unbind)';

	// SEARCH
	if($_REQUEST["filter_type_search"])
	  $filter .= '(reqType=search)';

	// MODIFY
	if($_REQUEST["filter_type_modify"])
	  $filter .= '(reqType=modify)';

	// DELETE
	if($_REQUEST["filter_type_delete"])
	  $filter .= '(reqType=delete)';

	if($_REQUEST["filter_type_unbind"] or $_REQUEST["filter_type_search"] or $_REQUEST["filter_type_modify"] or $_REQUEST["filter_type_delete"])
	  $filter .= ')';

	// Filter on session ID
	if($_REQUEST["session"])
	  $filter .= '(reqSession='.$_REQUEST["session"].')';

	// Filter on LDAP result (is or is not)
	if(ereg('^[0-9]', $_REQUEST["result"])) {
	  if($_REQUEST["result_type"] == 'is')
		$filter .= '(reqResult='.$_REQUEST["result"].')';
	  elseif($_REQUEST["result_type"] == 'not')
		$filter .= '(!(reqResult='.$_REQUEST["result"].'))';
	}

	// Filter on start time
	// NOTE: This can't be done, because they require exact match.
	//       Instead, filter that out after retreiving all objects.

	$filter .= ')';
  } else
	// Do not filter output - retreive EVERYTHING!
	// TODO: We really need to limit the number of objects!
	//       I have 43464 objects between 20070304004918
	//       and 20070312003842. There's no way I'll be
	//       able to retreive all those within the scope
	//       of Apache/PHP!
	$filter = 'objectClass=*';

  if($filter == '(&)') {
	// Faulty filter - nothing to filter on.
	// Fall back to the 'no filter' filter.
	$filter = 'objectClass=*';
  }

  if(pql_get_define("PQL_CONF_DEBUG_ME"))
	echo "filter: '$filter'<br>";

  foreach($_SESSION["ACCESSLOG_OVERLAY"] as $dn) {
	$logs = $_pql->search($dn, $filter, 'ONELEVEL');
	if(is_array($logs) and !$logs[0]) {
	  // Flat object...
	  $logs = array($logs);
	}

	if(is_array($logs)) {
	  for($i=0; $i < count($logs); $i++)
		$log[] = $logs[$i];
	}
  }

  if($_REQUEST["start"]) {
	// Filter on reqStart - must do this manually because of
	// schema limitations...
	$tmp = array();

	$start_len = strlen($_REQUEST["start"]);
//	$_REQUEST["start"] = sprintf("%.14d", $_REQUEST["start"]); // Make sure we have the format: YYYYMMDDHHMMSS
	for($j=$start_len+1; $j <= 14; $j++)
	  $_REQUEST["start"] .= '0';

	if($_REQUEST["end"]) {
	  $end_len   = strlen($_REQUEST["end"]);

//	  $_REQUEST["end"] = sprintf("%.14d", $_REQUEST["end"]); // Make sure we have the format: YYYYMMDDHHMMSS
	  for($j=$start_len+1; $j <= 14; $j++)
		$_REQUEST["end"] .= '0';
	}

	// Go through each object, check the 'reqStart' attribute
	for($i=0; $log[$i]; $i++) {
	  $start = substr($log[$i]["reqstart"], 0, 14); // Extract: YYYYMMDDHHMMSS

	  if($_REQUEST["end"]) {
		// We have an end time as well.
		$end = substr($log[$i]["reqstart"], 0, 14); // Extract: YYYYMMDDHHMMSS

		if(($start >= $_REQUEST["start"]) and ($end <= $_REQUEST["end"]))
		  // Between start and end time.
		  $tmp[] = $log[$i];
	  } else {
		if($start >= $_REQUEST["start"])
		  // Later than start time.
		  $tmp[] = $log[$i];
	  }
	}

	// Overwrite the original values with the filtered ones.
	$log = $tmp;
  }
  
  require($_SESSION["path"]."/tables/config_ldap-accesslog.inc");
} else {
  $ldap = pql_get_subschemas();
  if($_REQUEST["type"]) {
	$type = $_REQUEST["type"];
	$tmp = $ldap; unset($ldap);
	
	$ldap[$type] = $tmp[$type];
	$attributetypes = $tmp['attributetypes'];
  } else {
	$attributetypes = $ldap['attributetypes'];
  }

  $j = 1; $oc_counter = 0;

  require($_SESSION["path"]."/tables/config_ldap-monitor.inc");
}
?>
  </body>
</html>
<?php
pql_flush();

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
