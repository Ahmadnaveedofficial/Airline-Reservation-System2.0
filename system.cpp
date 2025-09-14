#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <sstream>
using namespace std;
// Base User class
class User {
public:
    string username;
    string password;
    string name;
    string email;
    User(string uname, string pwd, string n, string e)
        : username(uname), password(pwd), name(n), email(e) {}
    virtual void viewProfile() const {
        system("cls");
        cout<<"\n\n\n\n\n\n\n "<<endl;
        cout<<"\t\t\t\t\t\t\t\t<----------User Profile---------->"<<endl;
        cout<<"\n\t\t\t\t\t\t\t\tUsername: "<<username<<endl;
        cout<<"\t\t\t\t\t\t\t\tName: "<< name<<endl;
        cout<<"\t\t\t\t\t\t\t\tEmail: "<<email<<endl;
    }
};

// Passenger class
class Passenger : public User {
public:
    int bookings[10];
    int bookingCount;

    Passenger(string uname="",string pwd="",string n="",string e="")
        : User(uname,pwd,n,e),bookingCount(0) {
        for (int i=0;i<10;i++) 
            bookings[i]=-1;
    }
    void viewProfile() const override {
        cout<<"\n\n\n\n\n\n\n "<<endl;
        cout<<"\t\t\t\t\t\t\t\t<----------Passenger Profile---------->"<<endl;
        cout<<"\n\t\t\t\t\t\t\t\tUsername: "<<username<<endl;
        cout<<"\t\t\t\t\t\t\t\tName: "<< name<<endl;
        cout<<"\t\t\t\t\t\t\t\tEmail: "<<email<<endl;;
    }
};

// Admin class
class Admin : public User {
public:
    Admin():User("admin","admin123","System Admin","admin@airline.com") {}
    void viewProfile() const override {
        cout<<"\n\n\n\n\n\n\n "<<endl;
        cout<<"\t\t\t\t\t\t\t\t<----------Admin Profile---------->"<<endl;
        cout<<"\n\t\t\t\t\t\t\t\tUsername: "<<username<<endl;
        cout<<"\t\t\t\t\t\t\t\tName: "<< name<<endl;
        cout<<"\t\t\t\t\t\t\t\tEmail: "<<email<<endl;;
    }
};

// Flight class
class Flight {
public:
    int id;
    string name;
    string source;
    string destination;
    string date;
    string time;
    int totalSeats;
    int availableSeats;
    float price;
    Flight(int i = 0,string n="",string s="",string d="",string dt="",string t="",int ts=0,float p=0.0f)
        : id(i),name(n),source(s),destination(d),date(dt),time(t),
          totalSeats(ts),availableSeats(ts),price(p){}
};

// Ticket class
class Ticket {
public:
    string ticketId;
    int flightId;
    string passengerUsername;
    string bookingDate;
    float totalPrice;
    bool isCancelled;

    Ticket(string tid="",int fid=0,string user="",string bdate="",float price=0.0f)
        : ticketId(tid),flightId(fid),passengerUsername(user),bookingDate(bdate),
          totalPrice(price),isCancelled(false){}
};

// AirlineSystem class
class AirlineSystem {
public:
    Passenger passengers[100];
    int passengerCount=0;

    Flight flights[100];
    int flightCount=0;

    Ticket tickets[200];
    int ticketCount=0;

    Passenger* currentPassenger=nullptr;
    bool adminLoggedIn=false;
    int flightIdCounter=1;

    // Find passenger by username
    Passenger* findPassenger(const string &username) {
        for(int i=0;i<passengerCount;i++){
        	 if(passengers[i].username==username)
                return &passengers[i];
		}
        return nullptr;
    }

    // Find flight by ID
    Flight* findFlight(int id) {
        for(int i=0;i<flightCount;i++){
            if(flights[i].id==id)
                return &flights[i];	
		}
        return nullptr;
    }

    // Find ticket by ID
    Ticket* findTicket(const string &ticketId) {
        for(int i=0;i<ticketCount;i++){
        	 if(tickets[i].ticketId==ticketId && !tickets[i].isCancelled)
                return &tickets[i];
		}
        return nullptr;
    }

    // Check if current passenger booked a flight
    bool hasBookedFlight(int id){
        for(int i=0;i<currentPassenger->bookingCount;i++){
        if(currentPassenger->bookings[i]==id)
           return true;	
		}
        return false;
    }

    // Generate ticket ID
    string generateTicketId() {
        srand(time(0));
        string tid="TKT";
        string charset="ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        for(int i=0;i<6;i++)
            tid+=charset[rand() % charset.length()];
        return tid;
    }

    // Get current date as string
    string getCurrentDate(){
        time_t now = time(0);
        tm* ltm = localtime(&now);
        return to_string(ltm->tm_mday)+"/" + to_string(ltm->tm_mon + 1) + "/" + to_string(ltm->tm_year + 1900);
    }
	    
    void saveFlightsToFile() {
        ofstream out("flights.txt");
        for (int i=0;i<flightCount;i++) {
            out<<flights[i].id<<","
                <<flights[i].name<<","
                <<flights[i].source<<","
                <<flights[i].destination<<","
                <<flights[i].date<<","
                <<flights[i].time<<","
                <<flights[i].totalSeats<<","
                <<flights[i].availableSeats<<","
                <<flights[i].price<<"\n";
        }
        out.close();
    }

    void loadFlightsFromFile() {
        ifstream in("flights.txt");
        string line;
        while (getline(in, line)) {
            stringstream ss(line);
            string temp;
            Flight f;
            getline(ss, temp, ','); 
			f.id = stoi(temp);
            getline(ss, f.name, ',');
            getline(ss, f.source, ',');
            getline(ss, f.destination, ',');
            getline(ss, f.date, ',');
            getline(ss, f.time, ',');
            getline(ss, temp, ','); 
			f.totalSeats = stoi(temp);
            getline(ss, temp, ','); 
			f.availableSeats = stoi(temp);
            getline(ss, temp, ','); 
			f.price = stof(temp);
            flights[flightCount++] = f;
            if (f.id >= flightIdCounter) flightIdCounter = f.id + 1;
        }
		  in.close();
    }

    void savePassengersToFile() {
        ofstream out("passengers.txt");
        for (int i=0;i<passengerCount;i++) {
            Passenger& p=passengers[i];
            out<<p.username<< ","<<p.password<<","<<p.name<<","<<p.email<< ","<<p.bookingCount;
            for(int j=0;j<p.bookingCount;j++) {
                out<<","<<p.bookings[j];
            }
            out<<"\n";
        }
        out.close();
    }

    void loadPassengersFromFile(){
        ifstream in("passengers.txt");
        string line;
        while (getline(in, line)) {
            stringstream ss(line);
            string temp;
            Passenger p;
            getline(ss, p.username, ',');
            getline(ss, p.password, ',');
            getline(ss, p.name, ',');
            getline(ss, p.email, ',');
            getline(ss, temp, ','); p.bookingCount = stoi(temp);
            for (int i = 0; i < p.bookingCount; i++) {
            getline(ss, temp, ',');
                p.bookings[i] = stoi(temp);
            }
            passengers[passengerCount++] = p;
        }
        in.close();
    }

    void saveTicketsToFile() {
        ofstream out("tickets.txt");
        for (int i= 0;i<ticketCount;i++){
            out<<tickets[i].ticketId<<","
                <<tickets[i].flightId<<","
                <<tickets[i].passengerUsername<<","
                <<tickets[i].bookingDate<<","
                <<tickets[i].totalPrice<<","
                <<(tickets[i].isCancelled?"1":"0")<<endl;
        }
        out.close();
    }

    void loadTicketsFromFile() {
        ifstream in("tickets.txt");
        string line;
        while (getline(in, line)) {
            stringstream ss(line);
            string temp;
            Ticket t;
            getline(ss, t.ticketId, ',');
            getline(ss, temp, ','); t.flightId = stoi(temp);
            getline(ss, t.passengerUsername, ',');
            getline(ss, t.bookingDate, ',');
            getline(ss, temp, ','); 
			t.totalPrice = stof(temp);
            getline(ss, temp, ','); 
			t.isCancelled = (temp == "1");
            
            if (!t.isCancelled) {
                tickets[ticketCount++] = t;
            }
        }
        in.close();
    }

    void displayTicketDetails(const Ticket& ticket) {
        cout<<"\n\n\n\n\n\n\n";
        cout<<"\t\t\t\t\t\t\t========== TICKET DETAILS ==========\n";
        cout<<"\t\t\t\t\t\t\tTicket ID: "<<ticket.ticketId<<endl;
        cout<<"\t\t\t\t\t\t\tFlight ID: "<<ticket.flightId<<endl;
        Flight* flight = findFlight(ticket.flightId);
        if (flight) {
            cout<<"\t\t\t\t\t\t\tFlight Name:" << flight->name<<endl;
            cout<<"\t\t\t\t\t\t\tRoute: "<<flight->source<< " to "<<flight->destination<<endl;
            cout<<"\t\t\t\t\t\t\tDate: "<<flight->date<<" at "<<flight->time<<endl;
        }
        Passenger* passenger = findPassenger(ticket.passengerUsername);
        if (passenger) {
            cout<<"\t\t\t\t\t\t\tPassenger: "<<passenger->name<<endl;
            cout<<"\t\t\t\t\t\t\tEmail: "<<passenger->email<<endl;
        }
        cout<<"\t\t\t\t\t\t\tBooking Date: "<<ticket.bookingDate<<endl;
        cout<<"\t\t\t\t\t\t\tTotal Price: $"<<ticket.totalPrice<<endl;
        cout<<"\t\t\t\t\t\t\t====================================\n";
    }

    void displayAvailableFlights() {
        cout<<"\n\n\n\n\n\n\n\t\t\t\t\t\tAvailable Flights:\n";
        cout<<"\t\t\t\t\t\tID\tName\tSource\tDestination\tDate\tTime\tAvailable\tPrice\n";
        for(int i=0;i<flightCount;i++){
            if(flights[i].availableSeats>0) {
                Flight& f=flights[i];
                cout<<"\t\t\t\t\t\t"
                    <<f.id<<"\t"
                    <<f.name<<"\t"
                    <<f.source<<"\t"
                    <<f.destination<<"\t"
                    <<f.date<<"\t"
                    <<f.time<<"\t"
                    <<f.availableSeats
                    <<"\t\t$"<<f.price<<endl;
            }
        }
    }

    // Remove booking  currentPassenger by flight ID
    bool removeBooking(int flightId){
        for (int i=0;i<currentPassenger->bookingCount;i++) {
            if(currentPassenger->bookings[i]==flightId) {
                // Shift left bookings array
                for(int j=i;j<currentPassenger->bookingCount-1;j++){
                    currentPassenger->bookings[j]=currentPassenger->bookings[j+1];
                }
                currentPassenger->bookingCount--;
                currentPassenger->bookings[currentPassenger->bookingCount]=-1;
                return true;
           }
        }
        return false;
    }

public:
    void aboutUs(){
        cout<<"\n\n\n\n\n\n\n"
            <<"\t\t\t\t\t<-------------------------------- About Us ------------------------------->\n"
            <<"\n\t\t\t\t\tWelcome to Hazel Blue Airline where we believe that every journey is more\n"
            <<"\t\t\t\t\tthan just reaching a destination. Your safety, connection, and comfort\n"
            <<"\t\t\t\t\tare our priorities. We believe that aviation is proof that with\n"
            <<"\t\t\t\t\tdetermination, we have the capacity to achieve the impossible.\n"
            <<"\n\t\t\t\t\tThis airline was founded by Five members whose names are given below:\n"
            <<"\t\t\t\t\tAhmad, Saqib, Umer, Unbreen, Ukasha with a passion for aviation and excellence.\n"
            <<"\t\t\t\t\tTogether, we navigate the skies with ease and comfort, creating unforgettable\n"
            <<"\t\t\t\t\tjourneys with you.\n"
            <<"\n\t\t\t\t\t<------------------------------------------------------------------------>\n";
    }

    void mainMenu(){
        int choice;
        do {
            system("cls");
            cout<<"\n\n\n\n\n\n\n "<<endl;
            cout<<"\t\t\t\t\t\t\t<---------- WELCOME TO OUR HAZEL BLUE AIRLINE ---------->\n"<<endl;
            cout<<"\t\t\t\t\t\t\t\t        [1] Passenger Login                   "<<endl;
            cout<<"\t\t\t\t\t\t\t\t        [2] Register New Passenger            "<<endl;
            cout<<"\t\t\t\t\t\t\t\t        [3] Admin Login                       "<<endl;
            cout<<"\t\t\t\t\t\t\t\t        [4] What We Are?                      "<<endl;
            cout<<"\t\t\t\t\t\t\t\t        [5] Exit                              "<<endl;
            cout<<"\n\n\n";
            cout<<"\t\t\t\tEnter your choice: ";
            cin>>choice;
            switch(choice) {
                case 1: {
                    system("cls");
                    passengerLogin();
                    system("pause");
                    break;
                } 
                case 2: {
                    system("cls");
                    registerPassenger();
                    system("pause");
                    break;
                }
                case 3: {
                    system("cls");
                    adminLogin(); 
                    system("pause");
                    break;
                }    
                case 4: {
                    system("cls");
                    aboutUs();
                    system("pause");
                    break;
                }         
                case 5: {
                    system("cls");
                    cout<<"\n\n\n\n\n\n\n\t\t\t\t\t\t\t\tThank you for using Hazel Blue Airline!\n"; 
                    break;
                }
                default: {
                    system("cls");
                    cout<<"\n\n\n\n\n\n\n\t\t\t\t\t\t\t\tInvalid choice! Please try again.\n";
                    system("pause");        
                }
            }
        } while(choice!=5); 
    }

    void passengerLogin() {
        string username, password;
        system("cls");
        cout<<"\n\n\n\n\n\n\n "<<endl;
        cout <<"\t\t\t\t\t\t\t\t<----------Passenger Login---------->"<<endl;
        cout<<"\n\t\t\t\t\t\t\t\tUsername: ";
        cin>>username;
        cout<<"\t\t\t\t\t\t\t\tPassword: ";
        cin>>password;
        Passenger* p=findPassenger(username);
        if(p && p->password==password){
            currentPassenger=p;
            passengerMenu();
        } else {
            cout<<"\n\t\t\t\t\t\t\t\t\tInvalid credentials.\n";
        }
    }

    void registerPassenger(){
        if (passengerCount>=100) {
            cout<<"\n\t\t\t\t\t\t\t\t\tPassenger limit reached. Cannot register more.\n";
            return;
        }
        string username, password, name, email;
        cout<<"\n\n\n\n\n\n\n "<<endl;
        cout <<"\t\t\t\t\t\t\t\t<----------Register New Passenger---------->"<<endl;
        cout<<"\n\t\t\t\t\t\t\t\tUsername: ";
        cin>>username;
        if (findPassenger(username)!=nullptr) {
            cout<<"\n\t\t\t\t\t\t\t\tUsername already exists.\n";
            return;
        }
        cout<<"\t\t\t\t\t\t\t\tPassword: ";
        cin>>password;
        cout<<"\t\t\t\t\t\t\t\tName: ";
        cin.ignore();
        getline(cin, name);
        cout<<"\t\t\t\t\t\t\t\tEmail: ";
        getline(cin, email);
        passengers[passengerCount++]=Passenger(username, password, name, email);
        cout<<"\n\t\t\t\t\t\t\t\t\tRegistration successful.\n";
        savePassengersToFile();
    }

    void adminLogin() {
        string username, password;
        cout<<"\n\n\n\n\n\n\n "<<endl;
        cout<<"\t\t\t\t\t\t\t\t<----------Admin Login---------->"<<endl;
        cout<<"\n\t\t\t\t\t\t\t\tUsername: ";
        cin>>username;
        cout<<"\t\t\t\t\t\t\t\tPassword: ";
        cin>>password;
        if(username=="admin" && password=="admin123"){
            adminLoggedIn=true;
            system("cls");
            adminMenu();
        } else {
            cout<<"\n\t\t\t\t\t\t\t\t Invalid admin credentials.\n";
        }
    }

    void adminMenu() {
        while (adminLoggedIn) {
            system("cls");
            cout<<"\n\n\n\n\n\n\n "<<endl;
            cout<<"\t\t\t\t\t\t\t\t<----------Admin Menu---------->"<<endl;
            cout<<"\n\t\t\t\t\t\t\t\t      [1] View Profile         "<<endl;
            cout<<"\t\t\t\t\t\t\t\t      [2] Add Flight           "<<endl;
            cout<<"\t\t\t\t\t\t\t\t      [3] View Flights         "<<endl;
            cout<<"\t\t\t\t\t\t\t\t      [4] View All Tickets     "<<endl;
            cout<<"\t\t\t\t\t\t\t\t      [5] Logout               "<<endl;
            cout<<"\n\t\t\t\tEnter your choice: ";
            int choice; 
            cin>>choice;
            switch (choice) {
                case 1: {
                    system("cls");
                    Admin admin;
                    admin.viewProfile();
                    system("pause");
                    break;
                }
                case 2: {
                    system("cls");
                    addFlightMenu();
                    system("pause");
                    break;
                } 
                case 3: {
                    system("cls");
                    viewAllFlights();
                    system("pause");
                    break;
                }
                case 4: {
                    system("cls");
                    viewAllTickets();
                    system("pause");
                    break;
                }
                case 5: {
                    system("cls");
                    adminLoggedIn = false; 
                    cout<<"\n\n\n\n\n\n\n\n\t\t\t\t\t\t\t\t\tLogged out from admin.\n";
                    break;
                } 
                default: {
                    system("cls");
                    cout<<"\n\n\n\n\n\n\n\t\t\t\t\t\t\t\tInvalid choice! Please try again.\n";
                    system("pause");
                    break;
                } 
            }
        }
    }

    void addFlightMenu(){
        if (flightCount>=100) {
            cout<<"\n\n\n\n\n\n\n\t\t\t\t\t\t\t\tFlight capacity full. Cannot add more flights.\n";
            return;
        }
        string name, source, dest, date, time;
        int seats;
        float price;
        cout<<"\n\n\n\n\n\n\n "<<endl;
        cout<<"\t\t\t\t\t\t\t\t<----------Add New Flight---------->\n\n";
        cout<<"\t\t\t\t\t\t\t\tFlight Name: ";
        cin.ignore();
        getline(cin, name);
        cout<<"\t\t\t\t\t\t\t\tSource: ";
        getline(cin, source);
        cout<<"\t\t\t\t\t\t\t\tDestination: ";
        getline(cin, dest);
        cout<<"\t\t\t\t\t\t\t\tDate (DD/MM/YYYY): ";
        getline(cin, date);
        cout<<"\t\t\t\t\t\t\t\tTime (HH:MM): ";
        getline(cin, time);
        cout<<"\t\t\t\t\t\t\t\tTotal Seats: ";
        cin>>seats;
        cout<<"\t\t\t\t\t\t\t\tPrice per seat: ";
        cin>>price;

        flights[flightCount++] = Flight(flightIdCounter++, name, source, dest, date, time, seats, price);
        cout<<"\n\t\t\t\t\t\t\t\tFlight added successfully.\n";
        saveFlightsToFile();
    }

    void viewAllFlights() {
        cout<<"\n\n\n\n\n\n\n "<<endl;
        cout<<"\t\t\t\t\t\tAll Flight\n";
        cout<<"\t\t\t\t\t\tID\tName\tSource\tDestination\tDate\tTime\tAvailable\tPrice\n";
        for (int i = 0; i < flightCount; i++) {
            Flight& f = flights[i];
            cout<<"\t\t\t\t\t\t"<<f.id<<"\t" 
                <<f.name<<"\t"
                <<f.source<<"\t" 
                <<f.destination<<"\t" 
                <<f.date<<"\t" 
                <<f.time<<"\t"
                <<f.availableSeats<<"\t\t$" 
                <<f.price<<"\n";
        }
    }

    void viewAllTickets(){
        if (ticketCount == 0) {
            cout<<"\n\n\n\n\n\n\n\t\t\t\t\t\t\t\tNo tickets found.\n";
            return;
        }
        cout<<"\n\n\n\n\n\n\n\t\t\t\t\t\t\t\tAll Tickets:\n";
        for (int i = 0; i < ticketCount; i++) {
            if (!tickets[i].isCancelled) {
                displayTicketDetails(tickets[i]);
            }
        }
    }

    void passengerMenu() {
        while (currentPassenger) {
            system("cls");
            cout<<"\n\n\n\n\n\n\n "<<endl;
            cout<<"\t\t\t\t\t\t\t\t<---------- Passenger Menu ---------->\n"<<endl;
            cout<<"\t\t\t\t\t\t\t\t        [1] View Profile                   "<<endl;
            cout<<"\t\t\t\t\t\t\t\t        [2] View Available Flights         "<<endl;
            cout<<"\t\t\t\t\t\t\t\t        [3] Book Ticket                    "<<endl;
            cout<<"\t\t\t\t\t\t\t\t        [4] View My Bookings               "<<endl;
            cout<<"\t\t\t\t\t\t\t\t        [5] View Ticket Details            "<<endl;
            cout<<"\t\t\t\t\t\t\t\t        [6] Cancel Booking                 "<<endl;
            cout<<"\t\t\t\t\t\t\t\t        [7] Logout                         "<<endl;
            cout<<"\n\t\t\t\tEnter your choice: ";
            int choice;
            cin>>choice;
            switch (choice) {
                case 1: {
                    system("cls");
                    currentPassenger->viewProfile();
                    system("pause");
                    break;
                } 
                case 2: {
                    system("cls");
                    displayAvailableFlights();
                    system("pause"); 
                    break;
                } 
                case 3: {
                    system("cls");
                    bookTicket(); 
                    system("pause");
                    break;
                } 
                case 4: {
                    system("cls");
                    viewMyBookings();
                    system("pause");
                    break;
                }
                case 5: {
                    system("cls");
                    viewTicketDetails();
                    system("pause");
                    break;
                }
                case 6: {
                    system("cls");
                    cancelBooking();
                    system("pause");
                    break;
                } 
                case 7: {
                    system("cls");
                    currentPassenger = nullptr; 
                    cout<<"\n\n\n\n\n\n\n\n\t\t\t\t\t\t\t\t\tLogged out from Passenger.\n";
                    break;
                } 
                default: {
                    system("cls");
                    cout<<"\n\n\n\n\n\n\n\t\t\t\t\t\t\t\tInvalid choice! Please try again.\n";
                    system("pause");
                    break;
                }
            }
        }
    }

    void bookTicket() {
        if (currentPassenger->bookingCount>=10) {
            cout<<"\n\n\n\n\n\n\n\t\t\t\t\t\t\t\tBooking limit reached. Cancel existing booking to add new.\n";
            return;
        }
        displayAvailableFlights();
        cout<<"\n\t\t\t\t\t\tEnter Flight ID to book: ";
        int fid; 
        cin>>fid;
        Flight* flight = findFlight(fid);
        if (!flight) {
            cout<<"\n\n\n\n\n\n\t\t\t\t\t\t\tInvalid Flight ID.\n";
            return;
        }
        if (flight->availableSeats<=0) {
            cout<<"t\t\t\t\t\t\tNo seats available on this flight.\n";
            return;
        }
        if (hasBookedFlight(fid)) {
            cout<<"\n\n\n\t\t\t\t\t\t\tYou have already booked this flight.\n";
            return;
        }
        // Book flight
        flight->availableSeats--;
        currentPassenger->bookings[currentPassenger->bookingCount++] = fid;

        string tid = generateTicketId();
        string bookingDate = getCurrentDate();
        float totalPrice = flight->price;

        tickets[ticketCount++] = Ticket(tid, fid, currentPassenger->username, bookingDate, totalPrice);
        system("cls");
        cout<<"\n\n\n\n\n\n\n\t\t\t\t\t\t\tTicket booked successfully! Ticket ID: "<<tid;
        // Save ticket to file
        saveTicketsToFile();
        displayTicketDetails(tickets[ticketCount - 1]);
        savePassengersToFile();
        saveFlightsToFile();
    }

    void viewMyBookings(){
        if (currentPassenger->bookingCount==0){
            cout<<"\n\n\n\n\n\n\n\t\t\t\t\t\t\tYou have no bookings.\n";
            return;
        }
        cout<<"\n\n\n\n\n\n\n\t\t\t\t\t\t\tYour Bookings:\n";
        for (int i=0;i<currentPassenger->bookingCount;i++) {
            int fid=currentPassenger->bookings[i];
            Flight* f=findFlight(fid);
            if (f) {
                cout<<"\t\t\t\t\t\t\tFlight ID: "<<f->id<<", "<<f->name<<", From "<<f->source<<" to "<<f->destination 
                    <<", Date: "<<f->date<<", Time: "<<f->time<<", Price: $"<<f->price<<"\n";
            }
        }
    }
    void viewTicketDetails(){
        if (currentPassenger->bookingCount==0) {
            cout<<"\n\n\n\n\n\n\n\t\t\t\t\t\t\t\tYou have no bookings.\n";
            return;
        }

        // First show all tickets for this passenger
        bool foundAny=false;
        for (int i=0;i<ticketCount;i++){
            if (tickets[i].passengerUsername==currentPassenger->username&&!tickets[i].isCancelled){
                Flight* flight=findFlight(tickets[i].flightId);
                if(flight){
                    cout<<"\n\n\t\t\t\t\t\t\t\tTicket ID: "<<tickets[i].ticketId
                         <<"\n\t\t\t\t\t\t\t\tFlight: "<<flight->name<<" (" 
                         <<flight->source << " to "<<flight->destination<<")"
                         <<"\n\t\t\t\t\t\t\t\tDate: "<<flight->date<<" at "<<flight->time
                         <<"\n\t\t\t\t\t\t\t\t-----------------------------------\n";
                    foundAny = true;
                }
            }
        }

        if(!foundAny) {
            cout<<"\n\n\n\n\n\n\n\t\t\t\t\t\t\t\tNo tickets found.\n";
            return;
        }

        cout<<"\n\t\t\t\t\t\t\t\tEnter Ticket ID to view details: ";
        string tid;
        cin>>tid;

        for (int i=0;i<ticketCount;i++){
            if (tickets[i].ticketId==tid&& 
                tickets[i].passengerUsername==currentPassenger->username&&
                !tickets[i].isCancelled) {
                system("cls");
                displayTicketDetails(tickets[i]);
                return;
            }
        }
        cout<<"\n\n\n\n\n\n\n\t\t\t\t\t\t\tTicket not found.\n";
    }

    void cancelBooking(){
        if (currentPassenger->bookingCount==0){
            cout<<"\n\n\n\n\n\n\n\t\t\t\t\t\t\tYou have no bookings to cancel.\n";
            return;
        }
        cout<<"\n\n\n\n\n\n\n\t\t\t\t\t\t\tYour bookings:\n";
        for(int i=0;i<currentPassenger->bookingCount;i++){
            int fid=currentPassenger->bookings[i];
            Flight* f=findFlight(fid);
            if(f){
                cout<<"\t\t\t\t\t\t\t"<<fid<<": "<<f->name<< " from "<<f->source<<" to "<<f->destination<<" on "<<f->date<< "\n";
            }
        }
        cout<<"\n\n\t\t\t\t\t\t\tEnter Flight ID to cancel: ";
        int fid; 
        cin>>fid;
        if(!hasBookedFlight(fid)){
            cout<<"\n\n\n\n\n\n\n\t\t\t\t\t\t\tYou have not booked this flight.\n";
            return;
        }
        // Cancel booking
        if (removeBooking(fid)){
            Flight* flight=findFlight(fid);
            if (flight) {
                flight->availableSeats++;
                // Mark ticket as cancelled
                for (int i=0;i<ticketCount;i++) {
                    if (tickets[i].flightId==fid && 
                        tickets[i].passengerUsername==currentPassenger->username) {
                        tickets[i].isCancelled=true;
                        break;
                    }
                }
                saveTicketsToFile();
                cout<<"\n\n\n\n\n\n\n\t\t\t\t\t\t\tBooking cancelled successfully.\n";
            }    
        } else {
            cout<<"\n\n\n\n\n\n\n\t\t\t\t\t\t\tError cancelling booking.\n";
        }
        savePassengersToFile();
        saveFlightsToFile();
    }
};

int main() {
    system("color F0");
    AirlineSystem system;
    system.loadFlightsFromFile();      
    system.loadPassengersFromFile();
    system.loadTicketsFromFile();
    system.mainMenu();
    return 0;
}
