<?php
// attribute plugin for
// default domain name (the default domainname for a domain)
// $Id: attrib.defaultdomain.inc,v 2.31.4.1 2004-11-19 08:43:57 turbo Exp $

// {{{ attribute_check(void)
function attribute_check() {
    global $error, $LANG;
    
    // is typed in ?
    if(empty($_REQUEST["defaultdomain"])) {
		$error[pql_get_define("PQL_ATTR_DEFAULTDOMAIN")] = $LANG->_('Missing');
		return false;
    }

    // don't use ".."
    if(preg_match("/(\.){2,}/", $_REQUEST["defaultdomain"])) {
		$error[pql_get_define("PQL_ATTR_DEFAULTDOMAIN")] = $LANG->_('Invalid');
		return false;
    }
    
    return true;
}
// }}}

// {{{ attribute_init(void)
function attribute_init() {
    $_REQUEST["defaultdomain"] = @$_REQUEST["oldvalue"];
}
// }}}

// {{{ attribute_print_form(void)
function attribute_print_form() {
    global $error, $LANG;

	$defaultdomain = $_REQUEST["defaultdomain"];
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo pql_complete_constant($LANG->_('Change %what% for domain %domain%'), array('what' => $LANG->_('default domain name'), 'domain' => $_REQUEST["orgname"])); ?></th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Default domain name')?></td>
          <td><?php echo pql_format_error_span($error[pql_get_define("PQL_ATTR_DEFAULTDOMAIN")]); ?><input type="text" name="defaultdomain" value="<?=$defaultdomain?>" size="20"></td>
        </tr>
<?php if(!function_exists("pql_control"))
		require("./include/pql_control.inc");

	  // Initiate a connection to the QmailLDAP/Controls DN
	  $_pql_control = new pql_control($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);
		
      // Get all QmailLDAP/Control hosts.
      $result = pql_get_dn($_pql_control->ldap_linkid, $_SESSION["USER_SEARCH_DN_CTR"],
                           '(&(cn=*)(objectclass=qmailControl))', 'ONELEVEL');
      for($i=0; $result[$i]; $i++)
        $hosts[] = pql_get_attribute($_pql_control->ldap_linkid, $result[$i], pql_get_define("PQL_ATTR_CN"));

	  if(is_array($hosts)) {
?>

        <tr></tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => pql_complete_constant($LANG->_('domain to %what% in following objects'), array('what' => pql_get_define("PQL_ATTR_LOCALS"))))); ?>:</td>
          <td>
<?php		if(!pql_get_define("PQL_CONF_CONTROL_AUTOADDLOCALS", $_REQUEST["rootdn"])) {
				$i = 0;

		  		foreach($hosts as $host) {
?>
            <input type="checkbox" name="<?=pql_get_define("PQL_ATTR_LOCALS")?>_<?=$i?>" value="<?=$host?>"><?=$host?><br>
<?php	  			$i++;
		  		}
?>
            <input type="checkbox" name="<?=pql_get_define("PQL_ATTR_LOCALS")?>_<?=$i?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('to all hosts'))); ?><br>
            <input type="hidden" name="<?=pql_get_define("PQL_ATTR_LOCALS")?>" value="<?=$i?>">
            &nbsp;<?=$LANG->_('If all left unset, no replication will take place')?>
<?php		} else {
				$cnt = count($hosts);
?>
              &nbsp;<?=$LANG->_('Automatic replication configured')?>
              <input type="hidden" name="<?pql_get_define("PQL_ATTR_LOCALS")?>_<?=$cnt?>" value="on">
              <input type="hidden" name="<?pql_get_define("PQL_ATTR_LOCALS")?>" value="<?=$cnt?>">
<?php		} ?>
          </td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => pql_complete_constant($LANG->_('domain to %what% in following objects'), array('what' => pql_get_define("PQL_ATTR_RCPTHOSTS"))))); ?>:</td>
          <td>
<?php		if(!pql_get_define("PQL_CONF_CONTROL_AUTOADDRCPTHOSTS", $_REQUEST["rootdn"])) {
		  		foreach($hosts as $host) {
?>
            <input type="checkbox" name="<?=pql_get_define("PQL_ATTR_RCPTHOSTS")?>_<?=$i?>" value="<?=$host?>"><?=$host?><br>
<?php	  			$i++;
		  		}
?>
            <input type="checkbox" name="<?=pql_get_define("PQL_ATTR_RCPTHOSTS")?>_<?=$i?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('to all hosts'))); ?><br>
            <input type="hidden" name="<?=pql_get_define("PQL_ATTR_RCPTHOSTS")?>" value="<?=$i?>">
            &nbsp;<?=$LANG->_('If all left unset, no replication will take place')?>
<?php 		} else {
				$cnt = count($hosts);
?>
              &nbsp;<?=$LANG->_('Automatic replication configured')?>
              <input type="hidden" name="<?=pql_get_define("PQL_ATTR_RCPTHOSTS")?>_<?=$cnt?>" value="on">
              <input type="hidden" name="<?=pql_get_define("PQL_ATTR_RCPTHOSTS")?>" value="<?=$cnt?>">
<?php 		} ?>
          </td>
        </tr>
<?php	} // if(is_array($hosts)) ?>
      </th>
    </table>

    <input type="hidden" name="submit"   value="2">
    <input type="hidden" name="attrib"   value="<?=$_REQUEST["attrib"]?>">
    <input type="hidden" name="oldvalue" value="<?=$defaultdomain?>">
    <input type="hidden" name="domain"   value="<?=urlencode($_REQUEST["domain"])?>">
    <input type="hidden" name="rootdn"   value="<?=urlencode($_REQUEST["rootdn"])?>">
    <input type="hidden" name="view"     value="<?=@$_REQUEST["view"]?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}
// }}}

// {{{ attribute_save(type)
function attribute_save($type) {
    global $_pql, $LANG;

    $oldvalue = @$_REQUEST["oldvalue"];
	$defaultdomain = @$_REQUEST["defaultdomain"];

	if($defaultdomain == $oldvalue) {
		// don't change if new value equal to old one
		$msg = pql_complete_constant($LANG->_('%what% unchanged'), array('what' => $LANG->_('Default domainname')));
		attribute_forward($msg);
		break;
	}
		
    switch($type) {
      case "add":
      case "modify":
		if(pql_modify_attribute($_pql->ldap_linkid, $_REQUEST["domain"], $_REQUEST["attrib"], 1, $defaultdomain)) {
			$msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
										 array('what' => $LANG->_('Default domainname', 4),
											   'new'  => $defaultdomain));
			
			require("./include/pql_control.inc");
			$_pql_control = new pql_control($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

			// -----------------------------------------
			if(pql_get_define("PQL_CONF_CONTROL_AUTOADDLOCALS", $_REQUEST["rootdn"])) {
				// update locals if control patch is enabled and we have decided to do auto replication
				$amount = $_POST[pql_get_define("PQL_ATTR_LOCALS")];
				if($amount) {
					if($_POST[pql_get_define("PQL_ATTR_LOCALS")."_".$amount] != 'on') {
						for($i=0; $i < $amount; $i++)
						  if($_POST[pql_get_define("PQL_ATTR_LOCALS")."_".$i])
							$hosts[] = $_POST[pql_get_define("PQL_ATTR_LOCALS")."_".$i];
						
						if(is_array($hosts)) {
							foreach($hosts as $host) {
								pql_modify_attribute($_pql_control->ldap_linkid,
													 pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
													 pql_get_define("PQL_ATTR_LOCALS"),
													 '', $defaultdomain);
							}
						}
					} else {
						// Change all hosts
						if($type == 'delete')
						  pql_modify_attribute($_pql_control->ldap_linkid,
											   pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
											   pql_get_define("PQL_ATTR_LOCALS"),
											   $delval, '');
						else
						  pql_modify_attribute($_pql_control->ldap_linkid,
											   pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
											   pql_get_define("PQL_ATTR_LOCALS"),
											   '', $defaultdomain);
					}
				} else {
					// We have not gone through the form for adding defaultDomain nor additionalDomainName.
					// This _MUST_ be a 'delete {default,additional} domain'...
					//
					// Delete ALL locals/rcpthosts on ALL QmailLDAP/Controls that lists this domain.
					pql_control_update_domains($_pql_control, $_REQUEST["rootdn"], $_SESSION["USER_SEARCH_DN_CTR"],
											   '*', array($delval, ''));
				}
			}

			// -----------------------------------------
			if(pql_get_define("PQL_CONF_CONTROL_AUTOADDRCPTHOSTS", $_REQUEST["rootdn"])) {
				// update rcpthosts if control patch is enabled and we have decided to do auto replication
				$amount = $_POST[pql_get_define("PQL_ATTR_RCPTHOSTS")];
				if($amount) {
					if($_POST[pql_get_define("PQL_ATTR_RCPTHOSTS")."_".$amount] != 'on') {
						for($i=0; $i < $amount; $i++)
						  if($_POST[pql_get_define("PQL_ATTR_RCPTHOSTS")."_".$i])
							$hosts[] = $_POST[pql_get_define("PQL_ATTR_RCPTHOSTS")."_".$i];
						
						if(is_array($hosts)) {
							foreach($hosts as $host)
							  if($type == 'delete')
								pql_modify_attribute($_pql_control->ldap_linkid,
													 pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
													 pql_get_define("PQL_ATTR_RCPTHOSTS"),
													 $delval, '');
							  else
								pql_modify_attribute($_pql_control->ldap_linkid,
													 pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
													 pql_get_define("PQL_ATTR_RCPTHOSTS"),
													 '', $defaultdomain);
						}
					} else {
						// Change all hosts
						if($type == 'delete')
						  pql_modify_attribute($_pql_control->ldap_linkid,
											   pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
											   pql_get_define("PQL_ATTR_RCPTHOSTS"),
											   $delval, '');
						else
						  pql_modify_attribute($_pql_control->ldap_linkid,
											   pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
											   pql_get_define("PQL_ATTR_RCPTHOSTS"),
											   '', $defaultdomain);
					}
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
// }}}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
