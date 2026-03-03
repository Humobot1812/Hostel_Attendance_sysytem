// Set your Google Sheet ID here
var SS = SpreadsheetApp.openById('YOUR_GOOGLE_SHEET_ID');
var timezone = "Asia/Kolkata"; // Correct timezone for New Delhi
var str = "";

// Main POST function
function doPost(e) {
  var parsedData;
  
  try { 
    parsedData = JSON.parse(e.postData.contents);
  } catch (f) {
    return ContentService.createTextOutput("Error parsing request: " + f.message);
  }

  if (!parsedData) {
    return ContentService.createTextOutput("Error! Empty or incorrect request body.");
  }

  // Extract data
  var sheet = SS.getSheetByName(parsedData.sheet_name);
  var dataArr = parsedData.values.split(",");

  var Curr_Date = Utilities.formatDate(new Date(), timezone, "MM/dd/yyyy");
  var Curr_Time = Utilities.formatDate(new Date(), timezone, "hh:mm:ss a");

  var studentID = dataArr[0];
  var firstName = dataArr[1];
  var lastName = dataArr[2];
  var phone = dataArr[3];
  var address = dataArr[4];
  var gateNumber = dataArr[5];

  var data = sheet.getDataRange().getValues();
  var row_number = 0;
  var time_in = "";
  var time_out = "";
  var gate_existing = "";

  // STEP 1: Check if student already exists with same ID & gate without timeout
  for (var i = 1; i < data.length; i++) {
    if (data[i][0] == studentID && data[i][3] == gateNumber) { 
      time_in = data[i][1];
      time_out = data[i][2];
      gate_existing = data[i][3];
      row_number = i + 1;
      break;
    }
  }

  // STEP 2: If student found but time_out is blank, mark time_out (means leaving)
  if (row_number > 0 && time_out === "") {
    sheet.getRange("C" + row_number).setValue(Curr_Time); // Set time out
    str = "Exit marked successfully for " + firstName + " at Gate " + gateNumber;
    SpreadsheetApp.flush();
    return ContentService.createTextOutput(str);
  }

  // STEP 3: Otherwise, record a new entry (means entering)
  sheet.insertRowBefore(2);
  sheet.getRange("A2").setValue(studentID);
  sheet.getRange("B2").setValue(Curr_Time);  // Time In
  sheet.getRange("C2").setValue("");          // Time Out remains blank
  sheet.getRange("D2").setValue(gateNumber);
  sheet.getRange("E2").setValue(Curr_Date);
  sheet.getRange("F2").setValue(firstName);
  sheet.getRange("G2").setValue(lastName);
  sheet.getRange("H2").setValue(phone);
  sheet.getRange("I2").setValue(address);
  
  SpreadsheetApp.flush();
  str = "Entry marked successfully for " + firstName + " at Gate " + gateNumber;
  return ContentService.createTextOutput(str);
}
