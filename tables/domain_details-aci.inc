<?php
// Setup the URL redirection
$URL  = "domain_edit_attributes.php?rootdn=".$url["rootdn"]."&domain=".$url["domain"];
$URL .= "&attrib=".pql_get_define("PQL_ATTR_LDAPACI")."&view=" . $_REQUEST["view"] . "&";

// We can be called either from either domain_detail.php or tables/user_details-aci.inc
if(!@empty($_REQUEST["user"])) {
    // Get the ACIs in this user object
    $aci = pql_aci_get($_REQUEST["user"]);

	$URL .= "user=".$url["user"];
} else {
    // Get the ACIs in this branch object
    $aci = pql_aci_get($url["domain"]);
}

if(!is_array($aci)) {
    die("No ACIs availible for this branch.");
}
?>
  <table cellspacing="0" cellpadding="4" border="0">
    <th colspan="8" align="left"><?php echo $LANG->_('Access control values')?>
      <tr class="title">
        <td class="title"><?php echo $LANG->_('OID')?></td>
        <td class="title"><?php echo $LANG->_('Scope')?></td>

        <td class="title"><?php echo $LANG->_('Action')?></td>
        <td class="title"><?php echo $LANG->_('Permissions')?></td>
        <td class="title"><?php echo $LANG->_('Target')?></td>

        <td class="title"><?php echo $LANG->_('Type')?></td>
        <td class="title"><?php echo $LANG->_('Subject')?></td>
        <td class="title"><?php echo $LANG->_('Options')?></td>
      </tr>

<?php for($i=0; $i < $aci['count']; $i++) {
		$url = $URL . pql_get_define("PQL_ATTR_LDAPACI") . "=" . urlencode($aci[$i]["LINE"]);
?>
      <tr class="<?php pql_format_table(); ?>">
        <td><?php echo $aci[$i]["OID"]?></td>
        <td><?php echo $aci[$i]["SCOPE"]?></td>

<?php 	foreach($aci[$i]["RIGHTS"] as $right) {
	 	   if(is_array($right)) {
			   if(preg_match('/\[/', $right[0]) and preg_match('/\]/', $right[0])) {
				   $right = preg_replace('\[', '', $right[0]);
				   $right = preg_replace('\]', '', $right);
				   
				   $right = "<i>$right</i>";
			   } else {
				   $right = $right[0];
			   }
		   }
?>
        <td><?php echo $right?></td>
<?php	} ?>

        <td><?php echo $aci[$i]["TYPE"]?></td>
        <td><?php echo $aci[$i]["SUBJECT"]?></td>
        <td align="center"><a href="<?php echo $url?>&submit=3&type=edit"><img src="images/edit.png" width="12" height="12" alt="<?php echo $LANG->_('Change value')?>" border="0"></a>&nbsp;&nbsp;<a href="<?php echo $url?>&submit=2&type=delete"><img src="images/del.png" width="12" height="12" alt="<?php echo $LANG->_('Delete value')?>" border="0"></a>&nbsp;&nbsp;<?php if($i==0) { ?><img src="images/spacer_12x12.png" width="12" height="12" border="0"><?php } else { ?><a href="<?php echo $url?>&submit=4&type=moveup"><img src="images/up.png" width="12" height="12" alt="move up" border="0"></a><?php } ?><?php if(@empty($aci[$i+1]["LINE"])) { ?><img src="images/spacer_12x12.png" width="12" height="12" border="0"><?php } else { ?><a href="<?php echo $url?>&submit=4&type=movedown"><img src="images/down.png" width="12" height="12" alt="move down" border="0"></a><?php } ?></td>
      </tr>
<?php } ?>

      <tr class="subtitle">
        <td colspan="8"><a href="<?php echo $URL?>type=add"><img src="images/edit.png" width="12" height="12" alt="" border="0"> <?php echo $LANG->_('Register new ACI')?></a></td>
      </tr>
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
