# CS3520 Final Project - Community Center Reservation System

Alina Gonzalez & Khushi Khan

**README.md CONTENTS**
i. Summary
ii. Testing instructions

## i. Reservation System

Newton Reservation System is a Reservation Manager that provides various user functionality. Users can create or log into existing accounts and access information such as:

- Today's scheduling
- All events from today until a week in advance
- Creating events if an Organizer
- Approving requested events if a Facility Manager
- Purchasing tickets
- Updating information
- exiting the program

This Reservation Management System has a save state feature in which it will save all users and events status when closed.

## ii. Testing

Please adhere by the following instructions to test these main features:

1. Displaying events
2. Switching users
3. Making reservations
4. Enforcing facility rules (Facility Manager)
5. Save state / data persistence
6. Buying/cancelling tickets

### Running the program

There is a `Makefile` within this program. Please run the following commands to begin the program!

`make clean`

`make`

`./sys`

This program was tested on Khoury Linux Servers and should have no problem compiling.

### 1. Displaying Events

To display events, please prepare a log in for yourself. You can also use the testing login and represents a **NON-RESIDENT** user. The login information is presented below:

`Username: test_user`
`Password: test_pass`

To display events, simply follow the instructions once you have logged in and select option 2 or 3 to display **a week** worths of events or today's schedule. Please take note that only reservations within **7** days of the current date will be shown.

### 2. Switching users

To switch users, you must log out using the main menu option, or exit the program which automatically logs a user out. You may do this by pressing 7 on the main menu to exit or 6 to log out. Once you restart the program, user will be prompted to log in again. You may create your own login, or utilize another example login provided below:

`Username: cbox`

`Password: box123`

This is an **ORGANIZER** login. This can be used for the third testing procedure, **Making Reservations**.

### 3. Making Reservations

Ensure you are logged in as either a self-created **ORGANIZER** user, or the user login provided in the example above. Once that has been done, select the 1st option in the main menu and fill out all required information. The event **will NOT** be displayed once created if you attempt to display all events. Events must first be **approved** by a **FACILITY MANAGER**. All events get approved in **FIRST COME FIRST SERVE ORDER**, or the order that they were requested in.

_Additional note: **CITY** and **ORGANIZATIONS** cannot book Wedding style event layouts._

### 4. Facility Manager Enforcement

To log in as a **FACILITY MANAGER**, first log out/exit the program and use the following credentials:

`Username: erigby24`

`Password: beatle123`

You may also create your own Facility Manager login. Once logged in, you may notice that the main menu has another option, `(0) Approve Pending Requests`. Select this option and you will be first prompted with **ALL CITY ORGANIZED EVENTS FIRST** followed by any non-city organized events. This is to ensure that we abide by the city organized events prioritization rule. Additionally, events that have time conflicts with existing **APPROVED** events will not be displayed to the Facility Manager, as they will automatically be marked as not approved as they have time conflicts within the space. Once the event is approved, it will be displayed when any user runs the display events option **(as long as the event is within a week)**.

### 5. Save State / Data Persistence

Log out and log back in as any of the users you have previously logged in as. Also please refer to any to the event_files folder and take note of how it updates once events are requested and approved. Additionally, please take note of how cancellation requests are processed. Information such as the time at which the cancellation request was made as well as the event the user would like to cancel are stored in files.

### 6. Making Payments/Cancelling Events & Buying/Refund Tickets

Only those who are organizers of events have permission to make payments to rent out space for an event as well as cancel events (in turn, request a refund). Once a user requests to cancel an event, the event is stored in a file as well as a vector of events. The Facility Manager oversees the approval of cancellation requests and issues the appropriate refund amount depending on the time the request was made.

Only residents and non-residents are able to buy/cancel tickets. Non-residents are only given permissions to buy tickets from public events that they are allowed to attend. However, residents are free to buy tickets from any public event. Refunds are issued based on the price of a ticket for an event.
