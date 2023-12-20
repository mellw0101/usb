use std::process::Command;
use std::thread;

fn main() {
    // Define a list of shell commands to execute
    let commands = vec!["ls -l", "echo Hello", "date"];

    // Create a vector to store the threads
    let mut threads = vec![];

    // Spawn a thread for each command
    for (index, cmd) in commands.iter().enumerate() {
        let thread_name = format!("Thread {}", index);
        let cmd = cmd.to_string();

        let thread = thread::spawn(move || {
            println!("{} is executing: {}", thread_name, cmd);

            // Execute the shell command
            let result = Command::new("sh").arg("-c").arg(&cmd).status();

            match result {
                Ok(status) if status.success() => {
                    println!("{} executed successfully", thread_name);
                }
                Ok(status) => {
                    eprintln!("{} failed with {:?}", thread_name, status.code());
                }
                Err(e) => {
                    eprintln!("Error executing {}: {:?}", thread_name, e);
                }
            }
        });

        threads.push(thread);
    }

    // Wait for all threads to finish
    for thread in threads {
        thread.join().unwrap();
    }
}
