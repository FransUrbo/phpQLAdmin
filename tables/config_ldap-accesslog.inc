    <form action="<?=$_SERVER["PHP_SELF"]?>" method="post"
      <table cellspacing="0" cellpadding="3" border="0">
        <th colspan="3" align="left"><?=$LANG->_('LDAP Access Log')?>
          <tr class="<?php pql_format_table(); ?>">
            <td class="title"><?=$LANG->_('Filter')?>:</td>
            <td>
              <table>
                <th>
                  <tr>
                    <td><?=$LANG->_('Type')?>&nbsp;</td>
                    <td>
                      &nbsp;<input type="checkbox" name="filter_type_bind"<?php if($_REQUEST["filter_type_bind"]) { echo " CHECKED"; } ?>><?=$LANG->_('Binds')?><br>
                      &nbsp;<input type="checkbox" name="filter_type_unbind"<?php if($_REQUEST["filter_type_unbind"]) { echo " CHECKED"; } ?>><?=$LANG->_('Unbinds')?><br>
                      &nbsp;<input type="checkbox" name="filter_type_search"<?php if($_REQUEST["filter_type_search"]) { echo " CHECKED"; } ?>><?=$LANG->_('Searches')?><br>
                      &nbsp;<input type="checkbox" name="filter_type_modify"<?php if($_REQUEST["filter_type_modify"]) { echo " CHECKED"; } ?>><?=$LANG->_('Modifications')?><br>
                      &nbsp;<input type="checkbox" name="filter_type_delete"<?php if($_REQUEST["filter_type_delete"]) { echo " CHECKED"; } ?>><?=$LANG->_('Deletions')?><br>
                    </td>
                  </tr>

                  <tr><td colspan="2"></td></tr>

                  <tr>
                    <td><?=$LANG->_('Session')?></td>
                    <td>#<input type="text" name="session" size="11" value="<?=$_REQUEST["session"]?>"></td>
                  </tr>

                  <tr><td colspan="2"></td></tr>

                  <tr>
                    <td><?=$LANG->_('LDAP Result value')?></td>
                    <td>&nbsp;&nbsp;
                      <select name="result_type">
                        <option value="is"<?php if($_REQUEST["result_type"] == 'is') { echo " SELECTED"; } ?>><?=$LANG->_('Is')?></option>
                        <option value="not"<?php if($_REQUEST["result_type"] == 'not') { echo " SELECTED"; } ?>><?=$LANG->_('Is Not')?></option>
                      </select>
                      <br>&nbsp;&nbsp;
                      <input type="text" name="result" size="11" value="<?=$_REQUEST["result"]?>">
                    </td>
                  </tr>

                  <tr><td colspan="2"></td></tr>

                  <tr>
                    <td><?=$LANG->_('Timeperiod')?></td>
		      <td>&nbsp;&nbsp;
                        <input type="text" name="start" size="14" value="<?=$_REQUEST["start"]?>">
                        <br>&nbsp;&nbsp;
                        <input type="text" name="end"   size="14" value="<?=$_REQUEST["end"]?>">
                    </td>
                  </tr>
                </th>
              </table>
            </td>

            <td>
              <input type="reset" name="action" value="&nbsp;<?=$LANG->_('Reset')?>&nbsp;">
              <br>
              <input type="submit" value="<?=$LANG->_('Search')?>">
            </td>
          </tr>
        </th>
      </table>

      <input type="hidden" name="filter" value="1">
      <input type="hidden" name="type"   value="<?=$_REQUEST["type"]?>">
    </form>

    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">
        <tr class="title">
          <td><!-- counter --></td>
          <td><?=$LANG->_('Start')?></td>
          <td><?=$LANG->_('End')?></td>
          <td><?=$LANG->_('Session')?></td>
          <td><?=$LANG->_('Type')?></td>
          <td><?=$LANG->_('Result')?></td>
          <td><?=$LANG->_('Who')?>/<?=$LANG->_('What')?></td>
        </tr>
<?php for($i=0; $log[$i]; $i++) {
	$reqstart = pql_format_timestamp($log[$i]["reqstart"]);
	$reqstart = preg_replace('/ /', '&nbsp;', $reqstart);
	$reqstart = preg_replace('/\+.*/', '', $reqstart);

	$reqend   = pql_format_timestamp($log[$i]["reqend"]);
	$reqend   = preg_replace('/ /', '&nbsp;', $reqend);
	$reqend   = preg_replace('/\+.*/', '', $reqend);
?>

        <tr class="<?=pql_format_table()?>">
          <td align="right"><?=$i?></td>
          <td><?=$reqstart?>&nbsp;&nbsp;</td>
          <td><?=$reqend?>&nbsp;&nbsp;</td>
          <td><?=$log[$i]["reqsession"]?></td>
          <td><?=$log[$i]["reqtype"]?></td>
          <td align="center"><?=$log[$i]["reqresult"]?></td>
          <td>
<?php	if($log[$i]["reqauthzid"]) { ?>
<?php	  if($log[$i]["reqdn"]) { ?>AuthzID:<?php } ?>
            <?=$log[$i]["reqauthzid"]?>
<?php	  if($log[$i]["reqdn"]) { ?><br><?php } ?>
<?php	} 

	if($log[$i]["reqtype"] == 'search') {
?>
            Scope: '<?=$log[$i]["reqscope"]?>', Filter: '<?=$log[$i]["reqfilter"]?>'
<?php	  if($log[$i]["reqdn"]) { ?><br><?php } ?>
<?php	} ?>
            <?=$log[$i]["reqdn"]?>
          </td>
        </tr>
<?php } ?>
      </th>
    </table>
