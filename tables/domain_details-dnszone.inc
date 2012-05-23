<?php require($_SESSION["path"]."/include/pql_bind9.inc");
      $zones = pql_bind9_get_zone($_REQUEST["domain"]);
      if(is_array($zones)) {
?>
  <table cellspacing="0" border="0" width="50%" cellpadding="0">
    <th colspan="3" align="left">
<?php	foreach($zones as $dname => $zone_data) {
			// {{{ Use the first zone as default
			// ie, if we're not called with a specific zone to show
			if(!@$_REQUEST["dns_domain_name"] and !@$dns_domain_name) {
			  $dns_domain_name = $dname;
			  $_REQUEST["dns_domain_name"] = $dns_domain_name;
			} elseif(@$_REQUEST["dns_domain_name"]) {
			  $dns_domain_name = $_REQUEST["dns_domain_name"];
			}

			if($dname == $_REQUEST["dns_domain_name"]) {
			  $mark = "&mark=yes";
			} else {
			  unset($mark);
			}
// }}}
?>
    <a href="<?php echo $_SERVER["PHP_SELF"]?>?rootdn=<?php echo $_REQUEST["rootdn"]?>&domain=<?php echo $_REQUEST["domain"]?>&view=<?php echo $_REQUEST["view"]?>&dns_domain_name=<?php echo $dname?>"><img alt="/ <?php echo $dname?> \" vspace="0" hspace="0" border="0" src="tools/navbutton.php?label=<?php echo urlencode($dname)?><?php echo $mark?>"></a>
<?php	} ?>
    </th>
  </table>

  <br>

<?php
require($_SESSION["path"]."/tables/domain_details-dnszone_soa.inc");
echo "<br>";
require($_SESSION["path"]."/tables/domain_details-dnszone_data.inc");
?>

<?php } else { ?>
  <br>
  <?php echo $LANG->_('No zones configure for this branch')?>.
<?php } ?>
  <br>
  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left"><?php echo $LANG->_('Actions')?></th>
      <tr class="<?php pql_format_table(); ?>">
        <td><a href="bind9_add.php?rootdn=<?php echo $_REQUEST["rootdn"]?>&domain=<?php echo $_REQUEST["domain"]?>&action=add&type=domain&view=<?php echo $_REQUEST["view"]?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('DNS zone'))); ?></a></td>
      </tr>
<?php if($dns_domain_name) { ?>
      <tr class="<?php pql_format_table(); ?>">
        <td><a href="bind9_del.php?rootdn=<?php echo $_REQUEST["rootdn"]?>&domain=<?php echo $_REQUEST["domain"]?>&dns_domain_name=<?php echo $dns_domain_name?>&action=del&type=domain&view=<?php echo $_REQUEST["view"]?>"><?php echo pql_complete_constant($LANG->_('Remove this %what%'), array('what' => $LANG->_('DNS zone'))); ?></a></td>
      </tr>
<?php	if($_SESSION["ALLOW_BRANCH_CREATE"]) { ?>

      <tr class="<?php pql_format_table(); ?>">
        <td><a href="tools/dnszonetemplate.php?rootdn=<?php echo $_REQUEST["rootdn"]?>&domain=<?php echo $_REQUEST["domain"]?>&defaultdomain=<?php echo $dns_domain_name?>&view=<?php echo $_REQUEST["view"]?>"><?php echo $LANG->_('Create DNS template zone file')?></a></td>
      </tr>
<?php	}
	  }
?>
    </th>
  </table>

<?php
/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
