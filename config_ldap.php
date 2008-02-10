<?php
// shows configuration of phpQLAdmin
// $Id: config_ldap.php,v 1.26 2007-03-12 10:09:39 turbo Exp $
//
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");
require($_SESSION["path"]."/header.html");

$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

if($_REQUEST["type"] == 'config') {
  // {{{ LDAP Server configuration
  require($_SESSION["path"]."/include/pql_status.inc");

  if(!$_REQUEST["view"] or ($_REQUEST["view"] == 'global')) {
    if($_pql->ldap_config) {
      // Get the base cn=config values
      $configs = $_pql->search($_pql->ldap_config, 'objectClass=*', 'BASE');

	  // Get modules
	  $tmp = $_pql->search($_pql->ldap_config, 'cn=module*', 'ONELEVEL');
	  if(is_array($tmp)) {
        $configs += array(pql_get_define("PQL_ATTR_OLC_MODULE_PATH") => $tmp[pql_get_define("PQL_ATTR_OLC_MODULE_PATH")]);
        $configs += array(pql_get_define("PQL_ATTR_OLC_MODULE_LOAD") => $tmp[pql_get_define("PQL_ATTR_OLC_MODULE_LOAD")]);
	  }
	}

	// {{{ Cleanup and remove attribute of no interest
	if(is_array($configs)) {
      unset($configs['dn']);
	  unset($configs[pql_get_define("PQL_ATTR_CN")]);
	  unset($configs[pql_get_define("PQL_ATTR_OBJECTCLASS")]);
	  unset($configs[pql_get_define("PQL_ATTR_ATTRIBUTE_TYPES")]);

	  $tmp = $configs;
	  unset($configs);
	  $configs[0] = $tmp;
	} else
      $configs = array();
// }}}

	// {{{ Setup ints, bools etc
	$attributes = array('text'		=> array(pql_get_define("PQL_ATTR_OLC_ALLOWS"),
											 pql_get_define("PQL_ATTR_OLC_DB_URI"),
											 pql_get_define("PQL_ATTR_OLC_ATTRS_OPTS"),
											 pql_get_define("PQL_ATTR_OLC_AUTHZ_POLICY"),
											 pql_get_define("PQL_ATTR_OLC_AUTHZ_REGEXP"),
											 pql_get_define("PQL_ATTR_OLC_LOG_LEVEL"),
											 pql_get_define("PQL_ATTR_OLC_MODULE_PATH"),
											 pql_get_define("PQL_ATTR_OLC_MODULE_LOAD"),
											 pql_get_define("PQL_ATTR_OLC_SASL_HOST"),
											 pql_get_define("PQL_ATTR_OLC_SASL_REALM"),
											 pql_get_define("PQL_ATTR_OLC_SASL_SEC_PROPS"),
											 pql_get_define("PQL_ATTR_OLC_TLS_CRL_CHECK"),
											 pql_get_define("PQL_ATTR_OLC_TLS_VERIFY_CLIENT")),
						
						'integer'	=> array(pql_get_define("PQL_ATTR_OLC_CONCURRENCY"),
											 pql_get_define("PQL_ATTR_OLC_CONN_MAX_PEND"),
											 pql_get_define("PQL_ATTR_OLC_CONN_MAX_PEND_AUTH"),
											 pql_get_define("PQL_ATTR_OLC_IDLE_TIMEOUT"),
											 pql_get_define("PQL_ATTR_OLC_INDEX_INT_LEN"),
											 pql_get_define("PQL_ATTR_OLC_INDEX_SUBSTR_ANY_LEN"),
											 pql_get_define("PQL_ATTR_OLC_INDEX_SUBSTR_ANY_STEP"),
											 pql_get_define("PQL_ATTR_OLC_INDEX_SUBSTR_IF_MAX_LEN"),
											 pql_get_define("PQL_ATTR_OLC_INDEX_SUBSTR_IF_MIN_LEN"),
											 pql_get_define("PQL_ATTR_OLC_LOCAL_SSF"),
											 pql_get_define("PQL_ATTR_OLC_SIZE_LIMIT"),
											 pql_get_define("PQL_ATTR_OLC_SOCK_BUFF_MAX_INCOMING"),
											 pql_get_define("PQL_ATTR_OLC_SOCK_BUFF_MAX_INCOMING_AUTH"),
											 pql_get_define("PQL_ATTR_OLC_THREADS"),
											 pql_get_define("PQL_ATTR_OLC_TIME_LIMIT"),
											 pql_get_define("PQL_ATTR_OLC_TOOL_THREADS")),
						
						'files'		=> array(pql_get_define("PQL_ATTR_OLC_ARGS_FILE"),
											 pql_get_define("PQL_ATTR_OLC_CONFIG_FILE"),
											 pql_get_define("PQL_ATTR_OLC_PID_FILE"),
											 pql_get_define("PQL_ATTR_OLC_TLS_CA_CERT_FILE"),
											 pql_get_define("PQL_ATTR_OLC_TLS_CERT_FILE"),
											 pql_get_define("PQL_ATTR_OLC_TLS_CERT_KEY_FILE")),
						
						'bool'		=> array(pql_get_define("PQL_ATTR_OLC_GENTLE_HUP"),
											 pql_get_define("PQL_ATTR_OLC_READ_ONLY"),
											 pql_get_define("PQL_ATTR_OLC_REVERSE_LOOKUP")));
// }}}
  } elseif($_REQUEST["view"] == 'databases') {
    if($_pql->ldap_config) {
	  // Get modules
	  $configs = $_pql->search($_pql->ldap_config, 'olcDatabase=*', 'ONELEVEL');

      // {{{ Cleanup and remove attribute of no interest
      if(is_array($configs)) {
        for($i = 0; $configs[$i]; $i++) {
          unset($configs[$i]['dn']);
		  unset($configs[$i][pql_get_define("PQL_ATTR_CN")]);
		  unset($configs[$i][pql_get_define("PQL_ATTR_OBJECTCLASS")]);
		  unset($configs[$i][pql_get_define("PQL_ATTR_ATTRIBUTE_TYPES")]);

		  $configs[$i][pql_get_define("PQL_ATTR_OLC_DB")] = 
				  ereg_replace('^{[0-9]+}', '', $configs[$i][pql_get_define("PQL_ATTR_OLC_DB")]);

		  if($configs[$i][pql_get_define("PQL_ATTR_OLC_DB_CONFIG")]) {
            // Remove comments and empty lines from DB_CONFIG configuration
            for($j = 0; $configs[$i][pql_get_define("PQL_ATTR_OLC_DB_CONFIG")][$j]; $j++) {
              $configs[$i][pql_get_define("PQL_ATTR_OLC_DB_CONFIG")][$j] = 
					  ereg_replace('^{[0-9]+}', '', $configs[$i][pql_get_define("PQL_ATTR_OLC_DB_CONFIG")][$j]);

			  if(preg_match('/^#/', $configs[$i][pql_get_define("PQL_ATTR_OLC_DB_CONFIG")][$j]) ||
				 preg_match('/^$/', $configs[$i][pql_get_define("PQL_ATTR_OLC_DB_CONFIG")][$j]))
                unset($configs[$i][pql_get_define("PQL_ATTR_OLC_DB_CONFIG")][$j]);
			}

			// Cleanup and renumber the DB_CONFIG balues array.
			$configs[$i][pql_get_define("PQL_ATTR_OLC_DB_CONFIG")] =
					pql_uniq($configs[$i][pql_get_define("PQL_ATTR_OLC_DB_CONFIG")]);
		  }
		}
	  }
// }}}

      // {{{ Setup ints, bools etc
	  $attributes = array('ro'		=> array(pql_get_define("PQL_ATTR_OLC_DB"),
											 pql_get_define("PQL_ATTR_OLC_SCHEMA_DN")),
						  
						  'text'	=> array(pql_get_define("PQL_ATTR_OLC_ACCESS"),
											 pql_get_define("PQL_ATTR_OLC_DEFAULT_SEARCH_BASE"),
											 pql_get_define("PQL_ATTR_OLC_ROOT_DN"),
											 pql_get_define("PQL_ATTR_OLC_DB_SUFFIX"),
											 pql_get_define("PQL_ATTR_OLC_DB_INDEX"),
											 pql_get_define("PQL_ATTR_OLC_DB_CONFIG"),
											 pql_get_define("PQL_ATTR_OLC_DB_MODE"),
											 pql_get_define("PQL_ATTR_OLC_PASSWORD_HASH")),
						  
						  'integer'	=> array(pql_get_define("PQL_ATTR_OLC_MAX_DEREF_DEPTH"),
											 pql_get_define("PQL_ATTR_OLC_DB_CACHE_SIZE"),
											 pql_get_define("PQL_ATTR_OLC_SIZE_LIMIT"),
											 pql_get_define("PQL_ATTR_OLC_DB_IDL_CACHE_SIZE"),
											 pql_get_define("PQL_ATTR_OLC_DB_SEARCH_STACK"),
											 pql_get_define("PQL_ATTR_OLC_DB_SHM_KEY"),
											 pql_get_define("PQL_ATTR_OLC_DB_CACHE_FREE"),
											 pql_get_define("PQL_ATTR_OLC_DB_DN_CACHE_SIZE"),
											 pql_get_define("PQL_ATTR_OLC_DB_CHECK_POINT"),
											 pql_get_define("PQL_ATTR_OLC_TIME_LIMIT")),
						  
						  'files'	=> array(pql_get_define("PQL_ATTR_OLC_DB_DIRECTORY")),
						  
						  'bool'	=> array(pql_get_define("PQL_ATTR_OLC_ADD_CONNTENT_ACL"),
											 pql_get_define("PQL_ATTR_OLC_LAST_MOD"),
											 pql_get_define("PQL_ATTR_OLC_MONITORING"),
											 pql_get_define("PQL_ATTR_OLC_DB_NO_SYNC"),
											 pql_get_define("PQL_ATTR_OLC_DB_DIRTY_READ"),
											 pql_get_define("PQL_ATTR_OLC_DB_LINEAR_INDEX"),
											 pql_get_define("PQL_ATTR_OLC_READ_ONLY")));
// }}}
	}
  } elseif($_REQUEST["view"] == 'overlays') {
    if($_pql->ldap_config) {
	  // Get overlay info
	  $configs = $_pql->search($_pql->ldap_config, 'olcOverlay=*');

      // {{{ Cleanup and remove attribute of no interest
      if(is_array($configs)) {
        for($i = 0; $configs[$i]; $i++) {
          unset($configs[$i]['dn']);
		  unset($configs[$i][pql_get_define("PQL_ATTR_CN")]);
		  unset($configs[$i][pql_get_define("PQL_ATTR_OBJECTCLASS")]);
		  unset($configs[$i][pql_get_define("PQL_ATTR_ATTRIBUTE_TYPES")]);

		  $configs[$i][pql_get_define("PQL_ATTR_OLC_OVERLAY")] = 
				  ereg_replace('^{[0-9]+}', '', $configs[$i][pql_get_define("PQL_ATTR_OLC_OVERLAY")]);
		}
	  }
// }}}

      // {{{ Setup ints, bools etc
      $attributes = array('text'	=> array(pql_get_define("PQL_ATTR_OLC_OVERLAY"),
											 pql_get_define("PQL_ATTR_OLC_PPOLICY_DEFAULT"),
											 pql_get_define("PQL_ATTR_OLC_VAL_SORT_ATTR"),
											 pql_get_define("PQL_ATTR_OLC_UNIQ_BASE"),
											 pql_get_define("PQL_ATTR_OLC_UNIQ_ATTR"),
											 pql_get_define("PQL_ATTR_OLC_REFINT_ATTRS"),
											 pql_get_define("PQL_ATTR_OLC_ACCESSLOG_DB"),
											 pql_get_define("PQL_ATTR_OLC_ACCESSLOG_OPS"),
											 pql_get_define("PQL_ATTR_OLC_ACCESSLOG_PURGE"),
											 pql_get_define("PQL_ATTR_OLC_ACCESSLOG_OLD")),

						  'bool'	=> array(pql_get_define("PQL_ATTR_OLC_PPOLICY_HASH_CLEARTEXT"),
											 pql_get_define("PQL_ATTR_OLC_PPOLICY_USE_LOCKOUT")),

						  'files'	=> array(pql_get_define("PQL_ATTR_OLC_AUDIT_LOG_FILE")));
											 
											 
// }}}
	}
  }

  // {{{ Modify the values for prettier output
  for($i = 0; is_array($configs[$i]); $i++) {
    foreach($configs[$i] as $attrib => $val) {
      if(is_array($val)) {
        $new = array();

        foreach($val as $v) {
          $v = eregi_replace('^{[0-9]+}', '', $v);
          $v = eregi_replace('^{-[0-9]+}', '', $v);
          $v = eregi_replace('\" \"', '<br>&nbsp;&nbsp;', $v);
          $v = eregi_replace(' by ', '<br>&nbsp;&nbsp; by ', $v);
		  $v = eregi_replace(' attrs', '<br>&nbsp;&nbsp; attrs', $v);
          $v = eregi_replace('^\"', '', $v);
          $v = eregi_replace('\" \"', '', $v);

		  pql_add2array($new, $v);
        }

		$configs[$i][$attrib] = $new;
	  } else {
        // Value is not an array.
        $val = eregi_replace('^{[0-9]+}', '', $val);
		$val = eregi_replace('^{-[0-9]+}', '', $val);
		$val = eregi_replace('\" \"', '<br>&nbsp;&nbsp;', $val);
		$val = eregi_replace(' by ', '<br>&nbsp;&nbsp; by ', $val);
		$val = eregi_replace(' attrs', '<br>&nbsp;&nbsp; attrs', $val);
		$val = eregi_replace('^\"', '', $val);
		$val = eregi_replace('\" \"', '', $val);

		$configs[$i][$attrib] = $val;
	  }
	}
  }
// }}}

  // {{{ Default view for the details
  if(($_REQUEST["view"] == 'databases') and empty($_REQUEST["database"])) {
    if($configs[0][pql_get_define("PQL_ATTR_OLC_DB_SUFFIX")])
      $_REQUEST["database"] = $configs[0][pql_get_define("PQL_ATTR_OLC_DB_SUFFIX")];
	else
      $_REQUEST["database"] = $configs[0][pql_get_define("PQL_ATTR_OLC_DB")];
  } elseif(($_REQUEST["view"] == 'overlays') and empty($_REQUEST["overlay"]))
    $_REQUEST["overlay"] = $configs[0][pql_get_define("PQL_ATTR_OLC_OVERLAY")];
  elseif(empty($_REQUEST["view"]))
    $_REQUEST["view"] = 'global';
// }}}

  // {{{ Rearrange the array
  $new = array();
  for($i = 0; $configs[$i]; $i++) {
    if($_REQUEST["view"] == 'databases') {
      if($configs[$i][pql_get_define("PQL_ATTR_OLC_DB_SUFFIX")])
        $val = $configs[$i][pql_get_define("PQL_ATTR_OLC_DB_SUFFIX")];
      else
        $val = $configs[$i][pql_get_define("PQL_ATTR_OLC_DB")];

      $new[$val] = $configs[$i];
    } elseif($_REQUEST["view"] == 'overlays') {
      $val = $configs[$i][pql_get_define("PQL_ATTR_OLC_OVERLAY")];
      $new[$val] = $configs[$i];
    } else
      $new['global'] = $configs[0];
  }

  if($_REQUEST["view"] == 'databases') {
    $conf = $new[$_REQUEST['database']];
  } elseif($_REQUEST["view"] == 'overlays') {
    $conf = $new[$_REQUEST['overlay']];
  } else
    $conf = $new['global'];
// }}}
?>
    <span class="title1"><?php echo $LANG->_('LDAP server configuration')?></span>

    <br><br>
<?php
  $buttons = array('global&type=config'	=> 'Global configuration',
				   'databases&type=config'	=> 'Database configuration',
				   'overlays&type=config'	=> 'Overlay configuration');
  pql_generate_button($buttons);
?>
    <br>

    <table cellspacing="0" cellpadding="3" border="0">
<?php
  require($_SESSION["path"]."/tables/config_ldap-ldap.inc");
// }}}
} elseif($_REQUEST["type"] == 'accesslog') {
  // {{{ View the access logg

  if($_REQUEST["filter"]) {
    // {{{ Filter output

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
	if(preg_match('/^[0-9]/', $_REQUEST["result"])) {
	  if($_REQUEST["result_type"] == 'is')
		$filter .= '(reqResult='.$_REQUEST["result"].')';
	  elseif($_REQUEST["result_type"] == 'not')
		$filter .= '(!(reqResult='.$_REQUEST["result"].'))';
	}

	// Filter on start time
	// NOTE: This can't be done, because they require exact match.
	//       Instead, filter that out after retreiving all objects.

	$filter .= ')';

// }}}
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

  // -------------------------------
  if($_REQUEST["start"] && !@$_REQUEST["limit"]) {
	// Filter on reqStart - must do this manually because of
	// schema limitations...

	$start_len = strlen($_REQUEST["start"]);
//	$_REQUEST["start"] = sprintf("%.14d", $_REQUEST["start"]); // Make sure we have the format: YYYYMMDDHHMMSS
	for($j=$start_len+1; $j <= 14; $j++)
	  $_REQUEST["start"] .= '0';
  }

  // -------------------------------
  if($_REQUEST["end"] && !@$_REQUEST["limit"]) {
	// Filter on reqEnd
	$end_len   = strlen($_REQUEST["end"]);

//	$_REQUEST["end"] = sprintf("%.14d", $_REQUEST["end"]); // Make sure we have the format: YYYYMMDDHHMMSS
	for($j=$start_len+1; $j <= 14; $j++)
	  $_REQUEST["end"] .= '0';
  }

  // -------------------------------
  if(($_REQUEST["start"] || $_REQUEST["end"]) && !@$_REQUEST["limit"]) {
	$tmp = array();

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

// }}}
} else {
  // {{{ View objectclasses, attributes etc
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
// }}}
}
?>
  </body>
</html>
<?php
pql_flush();

/*
 * Local variables:
 * mode: php
 * tab-width: 4
 * End:
 */
?>
