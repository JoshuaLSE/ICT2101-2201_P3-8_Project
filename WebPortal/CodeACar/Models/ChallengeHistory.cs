using System;
using System.ComponentModel;

namespace CodeACar.Models
{
    public class ChallengeHistory
    {
        public int ChallengeHistoryId { get; set; }
        public bool IsCompleted { get; set; }
        public string Command { get; set; }
        public int ChallengeId { get; set; }
        public string Statistics { get; set; }
        [DisplayName("StudentId")]
        public int UserId { get; set; }

        public Challenge Challenge { get; set; }
        public User User { get; set; }
    }
}
