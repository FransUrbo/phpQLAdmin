<?php
// attribute plugin for
// default domain name (the default domainname for a domain)
// $Id: attrib.defaultdomain.inc,v 2.15 2003-05-08 12:28:26 turbo Exp $

function attribute_check(){
    global $defaultdomain, $domain, $oldvalue, $_pql, $error, $rootdn;
    
    // is typed in ?
    if($defaultdomain == ""){
		$error["defaultdomain"] = PQL_LANG_MISSING;
		return false;
    }
    
    // don't use ".."
    if(preg_match("/(\.){2,}/", $defaultdomain)){
		$error["defaultdomain"] = PQL_LANG_INVALID;
		return false;
    }
    
    return true;
}

function attribute_init(){
    global $defaultdomain, $oldvalue, $rootdn;
    
    $defaultdomain = $oldvalue;
}

function attribute_print_form(){
    global $defaultdomain, $domain, $PHP_SELF, $attrib, $oldvalue, $error, $rootdn;
	$oldvalue = $defaultdomain;
?>
  <form action="<?php echo $PHP_SELF ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo PQL_LANG_DOMAIN_DEFAULT_CHANGE_NAME . $domain; ?></th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?php echo PQL_LANG_DOMAIN_DEFAULT_NAME; ?></td>
          <td><?php echo format_error($error["defaultdomain"]); ?><input type="text" name="defaultdomain" value="<?php echo $defaultdomain; ?>" size="20"></td>
        </tr>

<?php if(!function_exists("pql_control"))
		require("./include/pql_control.inc");

	  // Initiate a connection to the QmailLDAP/Controls DN
	  $_pql_control = new pql_control($GLOBALS["USER_HOST"], $GLOBALS["USER_DN"], $GLOBALS["USER_PASS"]);

	  $hosts = pql_control_get_hosts($_pql_control->ldap_linkid, $GLOBALS["USER_SEARCH_DN_CTR"]);
	  if(is_array($hosts)) {
		  $i = 0;
?>
        <tr></tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Add domain to locals in following objects:</td>
          <td>
<?php	  foreach($hosts as $host) { ?>
            <input type="checkbox" name="locals_<?=$i?>" value="<?=$host?>"><?=$host?><br>
<?php	  	$i++;
		  }
?>
            <input type="checkbox" name="locals_<?=$i?>">Add to all hosts<br>
            <input type="hidden" name="locals" value="<?=$i?>">
            &nbsp;If all left unset, no replication will take place
          </td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Add domain to rcpthosts in following objects:</td>
          <td>
<?php	  foreach($hosts as $host) { ?>
            <input type="checkbox" name="rcpthosts_<?=$i?>" value="<?=$host?>"><?=$host?><br>
<?php	  	$i++;
		  }
?>
            <input type="checkbox" name="rcpthosts_<?=$i?>">Add to all hosts<br>
            <input type="hidden" name="rcpthosts" value="<?=$i?>">
            &nbsp;If all left unset, no replication will take place
          </td>
        </tr>

<?php } ?>
      </th>
    </table>

    <input type="hidden" name="submit" value="2">
    <input type="hidden" name="attrib" value="<?php echo $attrib; ?>">
    <input type="hidden" name="oldvalue" value="<?php echo $oldvalue; ?>">
    <input type="hidden" name="domain" value="<?php echo $domain; ?>">
    <input type="hidden" name="rootdn" value="<?=$rootdn?>">
    <input type="submit" value="<?php echo PQL_LANG_SAVE; ?>">
  </form>
<?php
}

function attribute_save($type){
    global $domain, $attrib, $oldvalue, $defaultdomain, $_pql, $config, $rootdn;
    
    // recontrol values
    if(!attribute_check($type))
	  return false;

	if($defaultdomain == $oldvalue) {
		// don't change if new value equal to old one
		$msg = PQL_LANG_DOMAIN_DEFAULT_CHANGE_UNCHANGED;
		attribute_forward($msg);
		break;
	}
		
    switch($type){
      case "add":
      case "modify":
		if(pql_set_domain_value($_pql->ldap_linkid, $domain, $attrib, $defaultdomain)){
			$msg = pql_complete_constant(PQL_LANG_DOMAIN_DEFAULT_CHANGE_OK, array("name" => $defaultdomain));
			
			require("./include/pql_control.inc");
			$_pql_control = new pql_control($GLOBALS["USER_HOST"], $GLOBALS["USER_DN"], $GLOBALS["USER_PASS"]);

			// -----------------------------------------
			// update locals if control patch is enabled
			$amount = $_POST["locals"];
			if($amount) {
				if($_POST["locals_$amount"] != 'on') {
					for($i=0; $i < $amount; $i++)
					  if($_POST["locals_$i"])
						$hosts[] = $_POST["locals_$i"];

					if(is_array($hosts)) {
						foreach($hosts as $host)
						  pql_control_update_domains($_pql_control, 'cn='.$host.','.$GLOBALS["USER_SEARCH_DN_CTR"],
													 '*', array($oldvalue, $defaultdomain), "locals");
					}
				} else {
					// Change all hosts
					if($type == 'delete')
					  pql_control_update_domains($_pql_control, $GLOBALS["USER_SEARCH_DN_CTR"],
												 '*', array($delval, ''), "locals");
					else
					  pql_control_update_domains($_pql_control, $GLOBALS["USER_SEARCH_DN_CTR"],
												 '*', array($oldvalue, $defaultdomain), "locals");
				}
			} else {
				// We have not gone through the form for adding defaultDomain nor additionalDomainName.
				// This _MUST_ be a 'delete {default,additional} domain'...
				//
				// Delete ALL locals/rcpthosts on ALL QmailLDAP/Controls that lists this domain.
				pql_control_update_domains($_pql_control, $GLOBALS["USER_SEARCH_DN_CTR"],
										   '*', array($delval, ''));
			}

			// -----------------------------------------
			// update rcpthosts if control patch is enabled
			$amount = $_POST["rcpthosts"];
			if($amount) {
				if($_POST["rcpthosts_$amount"] != 'on') {
					for($i=0; $i < $amount; $i++)
					  if($_POST["rcpthosts_$i"])
						$hosts[] = $_POST["rcpthosts_$i"];

					if(is_array($hosts)) {
						foreach($hosts as $host)
						  if($type == 'delete')
							pql_control_update_domains($_pql_control, 'cn='.$host.','.$GLOBALS["USER_SEARCH_DN_CTR"],
													   '*', array($delval, ''), "rcpthosts");
						  else
							pql_control_update_domains($_pql_control, 'cn='.$host.','.$GLOBALS["USER_SEARCH_DN_CTR"],
													   '*', array($oldvalue, $defaultdomain), "rcpthosts");
					}
				} else {
					// Change all hosts
					if($type == 'delete')
					  pql_control_update_domains($_pql_control, $GLOBALS["USER_SEARCH_DN_CTR"],
												 '*', array($delval, ''), "rcpthosts");
					else
					  pql_control_update_domains($_pql_control, $GLOBALS["USER_SEARCH_DN_CTR"],
												 '*', array($oldvalue, $defaultdomain), "rcpthosts");
				}
			}
		} else
		  $msg = PQL_LANG_DOMAIN_DEFAULT_CHANGE_FAILED . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
		
		attribute_forward($msg);
		break;
		
      default:
		die("unknown save type $type in " . __FILE__ . ", function save()");
    }
}

/*
 * Local variables:
 * mode: php
 * tab-width: 4
 * End:
 */
