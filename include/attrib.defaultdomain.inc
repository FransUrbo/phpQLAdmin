<?php
// attribute plugin for
// default domain name (the default domainname for a domain)
// $Id: attrib.defaultdomain.inc,v 2.22 2003-08-15 08:06:17 turbo Exp $

function attribute_check() {
    global $defaultdomain, $domain, $oldvalue, $_pql, $error, $rootdn, $LANG;
    
    // is typed in ?
    if($defaultdomain == "") {
		$error[pql_get_define("PQL_GLOB_ATTR_DEFAULTDOMAIN")] = $LANG->_('Missing');
		return false;
    }
    
    // don't use ".."
    if(preg_match("/(\.){2,}/", $defaultdomain)) {
		$error[pql_get_define("PQL_GLOB_ATTR_DEFAULTDOMAIN")] = $LANG->_('Invalid');
		return false;
    }
    
    return true;
}

function attribute_init() {
    global $defaultdomain, $oldvalue, $rootdn;
    $defaultdomain = $oldvalue;
}

function attribute_print_form() {
    global $defaultdomain, $domain, $PHP_SELF, $attrib, $oldvalue, $error, $rootdn, $view, $LANG;
	global $orgname;

	$oldvalue = $defaultdomain;
?>
  <form action="<?=$PHP_SELF?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo pql_complete_constant($LANG->_('Change %what% for domain %domain%'), array('what' => $LANG->_('default domain name'), 'domain' => $orgname)); ?></th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=$LANG->_('Default domain name')?></td>
          <td><?php echo format_error($error[pql_get_define("PQL_GLOB_ATTR_DEFAULTDOMAIN")]); ?><input type="text" name="defaultdomain" value="<?=$defaultdomain?>" size="20"></td>
        </tr>
<?php if(!function_exists("pql_control"))
		require("./include/pql_control.inc");

	  // Initiate a connection to the QmailLDAP/Controls DN
	  $_pql_control = new pql_control($GLOBALS["USER_HOST"], $GLOBALS["USER_DN"], $GLOBALS["USER_PASS"]);
		
	  $hosts = pql_control_get_hosts($_pql_control->ldap_linkid, $GLOBALS["USER_SEARCH_DN_CTR"]);
	  if(is_array($hosts)) {
?>

        <tr></tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('domain to '.pql_get_define("PQL_GLOB_ATTR_LOCALS").' in following objects'))); ?>:</td>
          <td>
<?php		if(!pql_get_define("PQL_GLOB_CONTROL_AUTOADDLOCALS")) {
				$i = 0;

		  		foreach($hosts as $host) {
?>
            <input type="checkbox" name="<?php echo pql_get_define("PQL_GLOB_ATTR_LOCALS")?>_<?=$i?>" value="<?=$host?>"><?=$host?><br>
<?php	  			$i++;
		  		}
?>
            <input type="checkbox" name="<?php echo pql_get_define("PQL_GLOB_ATTR_LOCALS")?>_<?=$i?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('to all hosts'))); ?><br>
            <input type="hidden" name="<?php echo pql_get_define("PQL_GLOB_ATTR_LOCALS")?>" value="<?=$i?>">
            &nbsp;<?=$LANG->_('If all left unset, no replication will take place')?>
<?php		} else {
				$cnt = count($hosts);
?>
              &nbsp;<?=$LANG->_('Automatic replication configured')?>
              <input type="hidden" name="<?php echo pql_get_define("PQL_GLOB_ATTR_LOCALS")?>_<?=$cnt?>" value="on">
              <input type="hidden" name="<?php echo pql_get_define("PQL_GLOB_ATTR_LOCALS")?>" value="<?=$cnt?>">
<?php		} ?>
          </td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('domain to '.pql_get_define("PQL_GLOB_ATTR_RCPTHOSTS").' in following objects'))); ?>:</td>
          <td>
<?php		if(!pql_get_define("PQL_GLOB_CONTROL_AUTOADDRCPTHOSTS")) {
		  		foreach($hosts as $host) {
?>
            <input type="checkbox" name="<?php echo pql_get_define("PQL_GLOB_ATTR_RCPTHOSTS")?>_<?=$i?>" value="<?=$host?>"><?=$host?><br>
<?php	  			$i++;
		  		}
?>
            <input type="checkbox" name="<?php echo pql_get_define("PQL_GLOB_ATTR_RCPTHOSTS")?>_<?=$i?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('to all hosts'))); ?><br>
            <input type="hidden" name="<?php echo pql_get_define("PQL_GLOB_ATTR_RCPTHOSTS")?>" value="<?=$i?>">
            &nbsp;<?=$LANG->_('If all left unset, no replication will take place')?>
<?php 		} else {
				$cnt = count($hosts);
?>
              &nbsp;<?=$LANG->_('Automatic replication configured')?>
              <input type="hidden" name="<?php echo pql_get_define("PQL_GLOB_ATTR_RCPTHOSTS")?>_<?=$cnt?>" value="on">
              <input type="hidden" name="<?php echo pql_get_define("PQL_GLOB_ATTR_RCPTHOSTS")?>" value="<?=$cnt?>">
<?php 		} ?>
          </td>
        </tr>
<?php	} // if(is_array($hosts)) ?>
      </th>
    </table>

    <input type="hidden" name="submit"   value="2">
    <input type="hidden" name="attrib"   value="<?=$attrib?>">
    <input type="hidden" name="oldvalue" value="<?=$oldvalue?>">
    <input type="hidden" name="domain"   value="<?=$domain?>">
    <input type="hidden" name="rootdn"   value="<?=$rootdn?>">
    <input type="hidden" name="view"     value="<?=$view?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}

function attribute_save($type) {
    global $domain, $attrib, $oldvalue, $defaultdomain, $_pql, $rootdn, $view, $LANG;
    
	if($defaultdomain == $oldvalue) {
		// don't change if new value equal to old one
		$msg = pql_complete_constant($LANG->_('%what% unchanged'), array('what' => $LANG->_('Default domainname')));
		attribute_forward($msg);
		break;
	}
		
    switch($type) {
      case "add":
      case "modify":
		if(pql_set_domain_value($_pql->ldap_linkid, $domain, $attrib, $defaultdomain)) {
			$msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
										 array('what' => $LANG->_('Default domainname', 4),
											   'new'  => $defaultdomain));
			
			require("./include/pql_control.inc");
			$_pql_control = new pql_control($GLOBALS["USER_HOST"], $GLOBALS["USER_DN"], $GLOBALS["USER_PASS"]);

			// -----------------------------------------
			// update locals if control patch is enabled
			$amount = $_POST[pql_get_define("PQL_GLOB_ATTR_LOCALS")];
			if($amount) {
				if($_POST[pql_get_define("PQL_GLOB_ATTR_LOCALS")."_".$amount] != 'on') {
					for($i=0; $i < $amount; $i++)
					  if($_POST[pql_get_define("PQL_GLOB_ATTR_LOCALS")."_".$i])
						$hosts[] = $_POST[pql_get_define("PQL_GLOB_ATTR_LOCALS")."_".$i];

					if(is_array($hosts)) {
						foreach($hosts as $host)
						  pql_control_update_domains($_pql_control, pql_get_define("PQL_GLOB_ATTR_CN").'=$host,'.$GLOBALS["USER_SEARCH_DN_CTR"],
													 '*', array($oldvalue, $defaultdomain),
													 pql_get_define("PQL_GLOB_ATTR_LOCALS"));
					}
				} else {
					// Change all hosts
					if($type == 'delete')
					  pql_control_update_domains($_pql_control, $GLOBALS["USER_SEARCH_DN_CTR"],
												 '*', array($delval, ''),
												 pql_get_define("PQL_GLOB_ATTR_LOCALS"));
					else
					  pql_control_update_domains($_pql_control, $GLOBALS["USER_SEARCH_DN_CTR"],
												 '*', array($oldvalue, $defaultdomain),
												 pql_get_define("PQL_GLOB_ATTR_LOCALS"));
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
			$amount = $_POST[pql_get_define("PQL_GLOB_ATTR_RCPTHOSTS")];
			if($amount) {
				if($_POST[pql_get_define("PQL_GLOB_ATTR_RCPTHOSTS")."_".$amount] != 'on') {
					for($i=0; $i < $amount; $i++)
					  if($_POST[pql_get_define("PQL_GLOB_ATTR_RCPTHOSTS")."_".$i])
						$hosts[] = $_POST[pql_get_define("PQL_GLOB_ATTR_RCPTHOSTS")."_".$i];

					if(is_array($hosts)) {
						foreach($hosts as $host)
						  if($type == 'delete')
							pql_control_update_domains($_pql_control, pql_get_define("PQL_GLOB_ATTR_CN").'=$host,'.$GLOBALS["USER_SEARCH_DN_CTR"],
													   '*', array($delval, ''),
													   pql_get_define("PQL_GLOB_ATTR_RCPTHOSTS"));
						  else
							pql_control_update_domains($_pql_control, pql_get_define("PQL_GLOB_ATTR_CN").'=$host,'.$GLOBALS["USER_SEARCH_DN_CTR"],
													   '*', array($oldvalue, $defaultdomain),
													   pql_get_define("PQL_GLOB_ATTR_RCPTHOSTS"));
					}
				} else {
					// Change all hosts
					if($type == 'delete')
					  pql_control_update_domains($_pql_control, $GLOBALS["USER_SEARCH_DN_CTR"],
												 '*', array($delval, ''),
												 pql_get_define("PQL_GLOB_ATTR_RCPTHOSTS"));
					else
					  pql_control_update_domains($_pql_control, $GLOBALS["USER_SEARCH_DN_CTR"],
												 '*', array($oldvalue, $defaultdomain),
												 pql_get_define("PQL_GLOB_ATTR_RCPTHOSTS"));
				}
			}
		} else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('Default domainname'))).": ".ldap_error($_pql->ldap_linkid);
		
		attribute_forward($msg);
		break;
		
      default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
    }
}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
